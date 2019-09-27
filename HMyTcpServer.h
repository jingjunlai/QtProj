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
    QMap<int, QThread*> m_mapThread;        //保存所有客户端线程 key 为客户端唯一的id
    QMap<int, int>      m_mapClientHeart;   //保存所有客户端心跳状态 key 为客户端唯一的id

    QTimer*             m_checkClientTimer; //检测客户端的定时器

protected:
    void incomingConnection(qintptr handle);    //重载

signals:
    void sigNewClient(int id);
    void sigSendData(int id, QString data); //向客户端发数据
    void sigDisconnectClient(int id);       //服务端主动断开某客户端，id = -1时表示断开所有连接的客户端；

private slots:
    void    sltCheckClientTimer();          //定时检测所有客户端
    void    sltClientHeartStatus(int id, int nHeartOutTimes);   //心跳状态

    void    sltClientDisconnect(int id);    //客户端主动断
    void    sltRecMessage(int id, int cmdID, QString data);
};

#endif // HMYTCPSERVER_H
