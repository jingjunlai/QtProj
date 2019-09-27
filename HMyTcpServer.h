#ifndef HMYTCPSERVER_H
#define HMYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include "HMyTcpWorker.h"
#include <QThread>
#include <QTimer>

#define HTCPCLIENT_HEART_TIMEOUT_COUNT 3

class HMyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit HMyTcpServer(QObject *parent = 0);
    ~HMyTcpServer();

    bool    startServer(int port);

private:
    QMap<int, QThread*> m_mapThread;        //�������пͻ����߳� key Ϊ�ͻ���Ψһ��id
    QMap<int, int>      m_mapClientHeart;   //�������пͻ�������״̬ key Ϊ�ͻ���Ψһ��id

    QTimer*             m_checkClientTimer; //���ͻ��˵Ķ�ʱ��

protected:
    void incomingConnection(qintptr handle);    //����

signals:
    void sigNewClient(int id);
    void sigSendData(int id, QString data); //��ͻ��˷�����
    void sigDisconnectClient(int id);       //����������Ͽ�ĳ�ͻ��ˣ�id = -1ʱ��ʾ�Ͽ��������ӵĿͻ��ˣ�

private slots:
    void    sltCheckClientTimer();          //��ʱ������пͻ���
    void    sltClientHeartStatus(int id, int nHeartOutTimes);   //����״̬

    void    sltClientDisconnect(int id);    //�ͻ���������
    void    sltRecMessage(int id, int cmdID, QString data);
};

#endif // HMYTCPSERVER_H
