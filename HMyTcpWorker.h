#ifndef HMYTCPWORKER_H
#define HMYTCPWORKER_H

#include <QObject>
#include "HMyTcpSocket.h"

class HMyTcpWorker : public QObject
{
    Q_OBJECT
public:
    explicit HMyTcpWorker(qintptr socketHandle, QObject *parent = 0);

private:
    HMyTcpSocket* m_tcpSocket;
    qintptr m_nID;

signals:
    void sigSendData2TcpSocket(QString data, int id);   //向客户端发数据
    void sigDisConnectTcpSocket(int id);                //断开某个客户端

    void sigClientDisconnect(int id);
    void sigClientHeartStatus(int id, int nHeartOutTimes);
    void sigRevMessage(int id, int cmdID, QString data);

public slots:
    void sltRevMessageFromClient();     //客户端发来数据处理
    void sltClientDisConnect();         //客户端发来断开信号

    void sltNewClient(int id);          //新客户端连接
    void sltSendData(int id, QString data); //向客户端发送数据
    void sltDisConnectClient(int id);       //服务端主动断开某客户端，当id = -1时表示断开全部
};

#endif // HMYTCPWORKER_H
