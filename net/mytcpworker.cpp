#include "mytcpworker.h"
#include "mytcpserver.h"
#include "cmsprotocolparser.h"
#include <QDebug>
#include <QXmlStreamReader>
#include <QThread>

MyTcpWorker::MyTcpWorker(MyTcpServer * const &p, qintptr socketHandle, QObject *parent)
    : QObject(parent),
      m_tcpSocket(Q_NULLPTR),
      m_nID(socketHandle)
{
    qRegisterMetaType<QVariant>("QVariant");
    m_Parser = new CmsProtocolParser();
    m_pTcpServer = p;
}

MyTcpWorker::~MyTcpWorker()
{
    qDebug() << "~MyTcpWorker ID = " << QThread::currentThreadId();
    if(m_Parser != Q_NULLPTR)
    {
//        m_Parser->deleteLater();
        delete m_Parser;
        m_Parser = Q_NULLPTR;
    }
    if(m_tcpSocket != Q_NULLPTR)
    {
        /*deleteLater 会断不开？*/
        delete m_tcpSocket;
        //m_tcpSocket->deleteLater();
        //m_tcpSocket = Q_NULLPTR;
    }
}

void MyTcpWorker::sltNewClient(int id)
{
    if(m_tcpSocket == NULL)
    {
        m_nID = id;

        m_tcpSocket = new MyTcpSocket(m_nID);

        connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(sltRevMessageFromClient()));
        connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(sltClientDisConnect()));
        connect(this, SIGNAL(sigDisConnectTcpSocket(int)), m_tcpSocket, SLOT(disConnectTcp(int)));
        connect(this, SIGNAL(sigSendData2TcpSocket(QString,int)), m_tcpSocket, SLOT(sendData(QString,int)));
        connect(this, SIGNAL(sigSendData2TcpSocket(const QByteArray &,int)), m_tcpSocket, SLOT(sendData(const QByteArray &,int)));

        qDebug() << "new Client" << m_nID << "MyTcpWorker ID = " << QThread::currentThreadId();
    }
}

void MyTcpWorker::sltRevMessageFromClient()
{
//    qDebug()<<"sltRevMessageFromClient ID = "<<QThread::currentThreadId();

    if(m_tcpSocket->bytesAvailable() <= 0)
    {
        qDebug() << "no data";
        return;
    }

    auto data = m_tcpSocket->readAll();
//    qDebug() << "tcp read data = " << data.toHex();
    if(data.isEmpty())
    {
        qDebug() << "tcp read data is empty";
        return;
    }

    m_Parser->appendData((unsigned char *)data.data(), data.size());
    QVector<QByteArray> packets = m_Parser->getLegalPackets();
//    QVariant value;
//    value.setValue(packets);
    QVariant *pv = new QVariant;
    pv->setValue(packets);
    if(!packets.isEmpty())
    {
        //m_RecevData += packets;
        //m_RecevData.clear();
        emit sendDataToServer(m_nID, pv);
        //m_pTcpServer->onReadMessage(m_nID, value);

    }
    else
    {
        delete pv;
        //qDebug() << "worker get packets is empty!";
    }
}

void MyTcpWorker::sltClientDisConnect()
{
    qDebug() << "client is disconnected" << m_nID;
    emit sigClientDisconnect(m_nID);
}

void MyTcpWorker::sltDisConnectClient(int id)
{
    if(m_tcpSocket)
    {
        if(id == m_nID || id == -1)
        {
            emit sigDisConnectTcpSocket(m_nID);
        }
    }
}

QByteArray MyTcpWorker::createPacket(const CmsMessageContent &message)
{
    QByteArray headerArr = message.header().toByteArray();
    QByteArray contentArr;
    foreach (auto c, message.contents())
    {
        if (!c.isNull())
            contentArr.append(c.data()->toByteArray());
    }

    char buf[2];
    buf[0] = (contentArr.size() >> 0) & 0xff;
    buf[1] = (contentArr.size() >> 8) & 0xff;
    QByteArray lengthArr(buf, sizeof(buf));

    QByteArray tmp = lengthArr + contentArr;
    tmp = headerArr + m_Parser->makePacket((const unsigned char *)tmp.data(), tmp.size());
    return tmp;
}

void MyTcpWorker::sltSendData(int id, QString data)
{
    if(m_tcpSocket)
    {
        if(id == m_nID)
        {
            emit sigSendData2TcpSocket(data, m_nID);
        }
    }
}

void MyTcpWorker::sltSendData(int id, const CmsMessageContent &message)
{
    if (m_tcpSocket == Q_NULLPTR)
        return;

    if(id == m_nID)
    {
        auto tmp = createPacket(message);

        emit sigSendData2TcpSocket(tmp, id);
    }
}


