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
    void sigSendData2TcpSocket(QString data, int id);   //��ͻ��˷�����
    void sigDisConnectTcpSocket(int id);                //�Ͽ�ĳ���ͻ���

    void sigClientDisconnect(int id);
    void sigClientHeartStatus(int id, int nHeartOutTimes);
    void sigRevMessage(int id, int cmdID, QString data);

public slots:
    void sltRevMessageFromClient();     //�ͻ��˷������ݴ���
    void sltClientDisConnect();         //�ͻ��˷����Ͽ��ź�

    void sltNewClient(int id);          //�¿ͻ�������
    void sltSendData(int id, QString data); //��ͻ��˷�������
    void sltDisConnectClient(int id);       //����������Ͽ�ĳ�ͻ��ˣ���id = -1ʱ��ʾ�Ͽ�ȫ��
};

#endif // HMYTCPWORKER_H
