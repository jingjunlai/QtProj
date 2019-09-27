#include "mytcpserver.h"
#include "cmsmessagecontent.h"
#include "cmsconverthelper.h"
#include "cmsecgcontenthandler.h"
#include "cmsspo2contenthandler.h"
#include "cmsrespcontenthandler.h"
#include "cmsnibpcontenthandler.h"
#include "cmsibpcontenthandler.h"
#include "cmstempcontenthandler.h"
#include "cmsco2contenthandler.h"
#include "cmscocontenthandler.h"
#include "cmsarrcontenthandler.h"
#include "cmscommandhandler.h"
#include "cmsheartbeat.h"
#include "mainwindow.h"


MyTcpServer::MyTcpServer(int iPort, QObject *parent)
    : QTcpServer(parent)
{
    m_iPort = iPort;
//    m_checkClientTimer = new QTimer(this);
//    connect(m_checkClientTimer, SIGNAL(timeout()), this, SLOT(sltCheckClientTimer()));

    m_pHeartTimer = new QTimer(this);
    connect(m_pHeartTimer, SIGNAL(timeout()), this, SLOT(sendHeartBeat()));

}

MyTcpServer::~MyTcpServer()
{
    qDebug() << "~MyTcpServer ID = " <<QThread::currentThreadId();

    QMap<int, MyTcpWorker *>::Iterator it;
    for(it = m_mapWorker.begin(); it != m_mapWorker.end(); ++it)
    {
        it.value()->deleteLater();
        it.value() = Q_NULLPTR;
    }

    QMap<int, QThread *>::Iterator it1;
    for(it1 = m_mapThread.begin(); it1 != m_mapThread.end(); ++it1)
    {
        it1.value()->quit();
        it1.value()->wait();
        it1.value()->deleteLater();
        it1.value() = Q_NULLPTR;
    }

    qDeleteAll(mParamHandler);
    delete m_pArrHandler;
    delete m_pCommandHandler;
}

bool MyTcpServer::startServer(int port)
{
    bool ret = true;
    ret = this->listen(QHostAddress::Any, port);

    if(ret)
    {
        //m_checkClientTimer->start(1000);
        emit enableMainWork(MainWindow::ServerID);
    }

    return ret;
}

void MyTcpServer::setModelPoint(Model * const &pModel)
{
    if(pModel != Q_NULLPTR)
        m_pModel = pModel;
}

void MyTcpServer::incomingConnection(qintptr handle)
{
    MyTcpWorker* worker = new MyTcpWorker(this, handle);
    QThread* thread = new QThread;
    worker->moveToThread(thread);

    qRegisterMetaType<CmsMessageContent>("CmsMessageContent");
    connect(this, SIGNAL(sigNewClient(int)), worker, SLOT(sltNewClient(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(sigSendData(int,QString)), worker, SLOT(sltSendData(int,QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(sigSendData(int,CmsMessageContent)), worker, SLOT(sltSendData(int,CmsMessageContent)), Qt::QueuedConnection);
//    connect(this, SIGNAL(sigDisconnectClient(int)), worker, SLOT(sltDisConnectClient(int)), Qt::QueuedConnection);
    connect(worker, SIGNAL(sigClientDisconnect(int)), this, SLOT(sltClientDisconnect(int)), Qt::QueuedConnection);
    connect(worker, SIGNAL(sigClientHeartStatus(int, int)), this, SLOT(sltClientHeartStatus(int, int)), Qt::QueuedConnection);
    connect(worker, SIGNAL(sigRevMessage(int,int,QString)), this, SLOT(sltRecMessage(int, int, QString)), Qt::QueuedConnection);
    connect(worker, SIGNAL(sendDataToServer(int, const QVariant*)), this, SLOT(onReadMessage(int, const QVariant*)), Qt::QueuedConnection);
    thread->start();

    m_mapThread[handle] = thread;
    m_mapWorker[handle] = worker;
    m_mapClientHeart[handle] = 0;
    m_iId = handle;
    emit sigNewClient(handle);

    m_pHeartTimer->start(500);
}

void MyTcpServer::sltCheckClientTimer()
{
    if(m_mapThread.size() < 0)
    {
        return;
    }

//    qDebug()<<QString::fromLocal8Bit("detect all clients, relese all src of clients");

    int nHeartOutTimes = 0;
    QMap<int, int>::iterator i;
    for(i = m_mapClientHeart.begin(); i != m_mapClientHeart.end();)
    {
        nHeartOutTimes = i.value();
        nHeartOutTimes++;
        i.value() = nHeartOutTimes;
        if(nHeartOutTimes > HTCPCLIENT_HEART_TIMEOUT_COUNT)
        {
            sltClientDisconnect(i.key());
            m_mapClientHeart.erase(i++);
        }
        else
        {
            i++;
        }
    }
}

void MyTcpServer::sltClientHeartStatus(int id, int nHeartOutTimes)
{
    QMap<int, int>::iterator i;
    for(i = m_mapClientHeart.begin(); i != m_mapClientHeart.end(); ++i)
    {
        if(i.key() == id)
        {
            i.value() = nHeartOutTimes;
            break;
        }
    }
}

void MyTcpServer::sltClientDisconnect(int id)
{
    emit sigDisconnectClient(id);

    /*清除worker和对应的Thread*/
    QMap<int, MyTcpWorker*>::iterator i = m_mapWorker.find(id);
    if (i != m_mapWorker.end())
    {
        i.value()->deleteLater();
        m_mapWorker.erase(i);
    }

    QMap<int, QThread*>::iterator k = m_mapThread.find(id);
    if (k != m_mapThread.end())
    {
        QThread *thread = k.value();
        if (thread)
        {
            if (thread->isRunning())
            {
                thread->quit();
                thread->wait();
            }
        }
        m_mapThread.erase(k);
    }
    qDebug() << QString::fromLocal8Bit("close thread of client, relese src");
}

void MyTcpServer::sltRecMessage(int id, int cmdID, QString data)
{
    Q_UNUSED(id);
    Q_UNUSED(cmdID);
    Q_UNUSED(data);
    qDebug()<<"sltRecMessage ID = "<<QThread::currentThreadId();
}

void MyTcpServer::onReadMessage(int Id, const QVariant *value)
{

    Q_UNUSED(Id);

    //QVector<QByteArray> packets = value.value<QVector<QByteArray>>();
    QVector<QByteArray> packets = value->value<QVector<QByteArray>>();

    foreach(auto a, packets)
    {
        CmsMessageContent content;
        content = a;

        //qDebug() << "content src data = " << content.receivedContentArray().toHex();

        handlePacket(content);
    }
    delete value;
}



void MyTcpServer::doWork()
{
    if(m_pModel == Q_NULLPTR)
    {
        qDebug() << "server do not connect to model : model not exist";
        return;
    }
    qRegisterMetaType<LabelID>("LabelID");
    /*注册各个CMS handler*/
    auto EcgHandler = new CmsEcgContentHandler(LABEL_ECG, this);
    mParamHandler.insert(LABEL_ECG, EcgHandler);
    connect(EcgHandler, SIGNAL(sendDataField(LabelID,const QVariant*)), m_pModel, SLOT(setModuleData(LabelID, const QVariant*)), Qt::QueuedConnection);
    connect(EcgHandler, SIGNAL(sendConfigField(LabelID,const QVariant*)), m_pModel, SLOT(setModuleConfig(LabelID, const QVariant*)), Qt::QueuedConnection);

    auto Spo2Handler = new CmsSpo2ContentHandler(LABEL_SPO2, this);
    mParamHandler.insert(LABEL_SPO2, Spo2Handler);
    connect(Spo2Handler, SIGNAL(sendDataField(LabelID,const QVariant*)), m_pModel, SLOT(setModuleData(LabelID, const QVariant*)), Qt::QueuedConnection);

    auto RespHandler = new CmsRespContentHandler(LABEL_RESP, this);
    mParamHandler.insert(LABEL_RESP, RespHandler);
    connect(RespHandler, SIGNAL(sendDataField(LabelID,const QVariant*)), m_pModel, SLOT(setModuleData(LabelID, const QVariant*)), Qt::QueuedConnection);

    auto NibpHandler = new CmsNibpContentHandler(LABEL_NIBP, this);
    mParamHandler.insert(LABEL_NIBP, NibpHandler);
    connect(NibpHandler, SIGNAL(sendDataField(LabelID,const QVariant*)), m_pModel, SLOT(setModuleData(LabelID, const QVariant*)), Qt::QueuedConnection);

    for(int i = LABEL_IBP_START; i <= LABEL_IBP_END; i++)
    {
        auto IbpHandler = new CmsIbpContentHandler((LabelID)i, this);
        mParamHandler.insert((LabelID)i, IbpHandler);
        connect(IbpHandler, SIGNAL(sendDataField(LabelID,const QVariant*)), m_pModel, SLOT(setModuleData(LabelID, const QVariant*)), Qt::QueuedConnection);
    }

    for(int i = LABEL_TEMP_A; i <= LABEL_TEMP_D; i++)
    {
        auto TempHandler = new CmsTempContentHandler((LabelID)i, this);
        mParamHandler.insert((LabelID)i, TempHandler);
        connect(TempHandler, SIGNAL(sendDataField(LabelID,const QVariant*)), m_pModel, SLOT(setModuleData(LabelID, const QVariant*)), Qt::QueuedConnection);
    }

    auto Co2Handler = new CmsCo2ContentHandler(LABEL_CO2, this);
    mParamHandler.insert(LABEL_CO2, Co2Handler);
    connect(Co2Handler, SIGNAL(sendDataField(LabelID,const QVariant*)), m_pModel, SLOT(setModuleData(LabelID, const QVariant*)), Qt::QueuedConnection);

    auto CoHandler = new CmsCoContentHandler(LABEL_CO, this);
    mParamHandler.insert(LABEL_CO, CoHandler);
    connect(CoHandler, SIGNAL(sendDataField(LabelID,const QVariant*)), m_pModel, SLOT(setModuleData(LabelID, const QVariant*)), Qt::QueuedConnection);

    m_pArrHandler = new CmsArrContentHandler(LABEL_ARR, this);
    connect(m_pArrHandler, SIGNAL(sendDataField(LabelID,const QVariant*)), m_pModel, SLOT(setModuleData(LabelID, const QVariant*)), Qt::QueuedConnection);

    m_pCommandHandler = new CmsCommandHandler(LABEL_SYSTEM, this);
    connect(m_pCommandHandler, SIGNAL(sendUserType(int)), m_pModel, SIGNAL(sendUserType(int)), Qt::QueuedConnection);
    this->startServer(m_iPort);
}

void MyTcpServer::transmitConfigData(int id, const QSharedPointer<ICmsData> data)
{
    if(data.isNull())
    {
        qDebug() << "transmit data is null!";
        return;
    }
    CmsMessageHeader header(0xF1, 0x06, CMS_MessageDownConfig);
    CmsMessageContent message;
    message.setHeader(header);
    message.addContent(data);

    emit sigSendData(id, message);
}

void MyTcpServer::transmitCommand(int id, const QSharedPointer<ICmsData> data)
{
    if(data.isNull())
    {
        qDebug() << "transmit data is null!";
        return;
    }
    CmsMessageHeader header(0xF1, 0x06, CMS_MessageDownCommand);
    CmsMessageContent message;
    message.setHeader(header);
    message.addContent(data);

    emit sigSendData(id, message);
}

void MyTcpServer::sendHeartBeat()
{
    CmsMessageHeader header(0xF1, 0x06, CMS_MessageDownHeartBeat);

    auto data = new CmsHeartBeat();

    QSharedPointer<ICmsData> pdata(data);
    CmsMessageContent message;
    message.setHeader(header);
    message.addContent(pdata);

    emit sigSendData(m_iId, message);

    pdata.clear();
}

void MyTcpServer::handlePacket(const CmsMessageContent &content)
{
    switch(content.header().messageType())
    {
        case CMS_MessageUpHeartBeat:
            {
                //qDebug() << "recevice CMS_MessageUpHeartBeat.";
            }
            break;
        case CMS_MessageUpModuleData:
            {
                //qDebug() << "recevice CMS_MessageUpModuleData.";
                int usedBytes = 0;
                auto stream = content.receivedContentArray();
                auto p = (const unsigned char*)stream.constData();
                while (stream.size() - usedBytes >= 4)
                {
                    CmsLabelID cmsLabel = (CmsLabelID)p[usedBytes + 1];
                    int size = (p[usedBytes + 2] << 0) + (p[usedBytes + 3] << 8);

                    int packetLen = size + 4;
                    auto label = CmsConvertHelper::convertToLocalLabelId(cmsLabel);
//                    qDebug() << label << packetLen;
                    if (mParamHandler.contains(label))
                    {
                        QByteArray tmp = stream.mid(usedBytes, packetLen);
                        mParamHandler.value(label)->handleContent(CMS_MessageUpModuleData, tmp);
                    }
                    usedBytes += packetLen;
                }
            }
            break;
        case CMS_MessageUpConfig:
            {
                //qDebug() << "recevice CMS_MessageUpConfig.";
                int usedBytes = 0;
                auto stream = content.receivedContentArray();
                auto p = (const unsigned char*)stream.constData();
                while (stream.size() - usedBytes >= 4)
                {
                    CmsLabelID cmsLabel = (CmsLabelID)p[usedBytes + 1];
                    int size = (p[usedBytes + 2] << 0) + (p[usedBytes + 3] << 8);

                    int packetLen = size + 4;
                    auto label = CmsConvertHelper::convertToLocalLabelId(cmsLabel);
                    //qDebug() << label << packetLen;
                    if (mParamHandler.contains(label))
                    {
                        QByteArray tmp = stream.mid(usedBytes, packetLen);
                        mParamHandler.value(label)->handleContent(CMS_MessageUpConfig, tmp);
                    }
                    usedBytes += packetLen;
                }
            }
            break;

        case CMS_MessageUpArrInfo:
            {
                int usedBytes = 0;
                auto stream = content.receivedContentArray();
                auto p = (const unsigned char*)stream.constData();
                while (stream.size() - usedBytes >= 2)
                {
                    int size = (p[usedBytes] << 0) + (p[usedBytes + 1] << 8);

                    int packetLen = size + 2;
                    QByteArray tmp = stream.mid(usedBytes, packetLen);
                    m_pArrHandler->handleContent(CMS_MessageUpArrInfo, tmp);
                    usedBytes += packetLen;
                }
            }
            break;

        case CMS_MessageUpCommand:
            {
                int usedBytes = 0;
                auto stream = content.receivedContentArray();
                auto p = (const unsigned char*)stream.constData();
                while (stream.size() - usedBytes >= 4)
                {
                    int size = (p[usedBytes + 2] << 0) + (p[usedBytes + 3] << 8);

                    int packetLen = size + 4;
                    QByteArray tmp = stream.mid(usedBytes, packetLen);
                    m_pCommandHandler->handleContent(CMS_MessageUpCommand, tmp);
                    usedBytes += packetLen;
                }
            }
            break;

        default:
            {
                //qDebug() << "unkown data type = " << QString::number(content.header().messageType(), 16);
                //qDebug() << "handlePacket unknow data";
            }
            break;
    }
}
