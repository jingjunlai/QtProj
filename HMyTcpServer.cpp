#include "HMyTcpServer.h"

HMyTcpServer::HMyTcpServer(QObject *parent)
    : QTcpServer(parent)
{
    m_checkClientTimer = new QTimer;
    connect(m_checkClientTimer, SIGNAL(timeout()), this, SLOT(sltCheckClientTimer()));
}

HMyTcpServer::~HMyTcpServer()
{

}

bool HMyTcpServer::startServer(int port)
{
    bool ret = true;
    ret = this->listen(QHostAddress::Any, port);

    if(ret)
    {
        //m_checkClientTimer->start(1000);
    }

    return ret;
}

void HMyTcpServer::incomingConnection(qintptr handle)
{
    HMyTcpWorker* worker = new HMyTcpWorker(handle);
    QThread* thread = new QThread(worker);
    worker->moveToThread(thread);

    //connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(sigNewClient(int)), worker, SLOT(sltNewClient(int)));
    connect(this, SIGNAL(sigSendData(int,QString)), worker, SLOT(sltSendData(int,QString)));
    connect(this, SIGNAL(sigDisconnectClient(int)), worker, SLOT(sltDisConnectClient(int)));
    connect(worker, SIGNAL(sigClientDisconnect(int)), this, SLOT(sltClientDisconnect(int)));
    connect(worker, SIGNAL(sigClientHeartStatus(int, int)), this, SLOT(sltClientHeartStatus(int, int)));
    connect(worker, SIGNAL(sigRevMessage(int,int,QString)), this, SLOT(sltRecMessage(int, int, QString)));
    thread->start();

    m_mapThread[handle] = thread;
    m_mapClientHeart[handle] = 0;
    emit sigNewClient(handle);
}

void HMyTcpServer::sltCheckClientTimer()
{
    if(m_mapThread.size() < 0)
    {
        return;
    }

    qDebug()<<QString::fromLocal8Bit("������пͻ��ˣ��ͷŶ��ߵĿͻ�����Դ");

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

void HMyTcpServer::sltClientHeartStatus(int id, int nHeartOutTimes)
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

void HMyTcpServer::sltClientDisconnect(int id)
{
    emit sigDisconnectClient(id);

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
    qDebug() << QString::fromLocal8Bit("�ر��̣߳��ͷ���Դ");
}

void HMyTcpServer::sltRecMessage(int id, int cmdID, QString data)
{
    //ĳ���ͻ��ˣ�ĳ������������ݣ���һ����������UI�Ľ�������������н��У�
    //�������յ������ĺ�ʱ������ȫ���߳��н��У�
}
