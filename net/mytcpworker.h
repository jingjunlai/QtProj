#ifndef HMYTCPWORKER_H
#define HMYTCPWORKER_H

#include <QObject>
#include <QVariant>
#include <QMetaType>
#include <QVector>
#include "mytcpsocket.h"
#include "cmsmessagecontent.h"

Q_DECLARE_METATYPE(QVector<QByteArray>)

class MyTcpServer;
class CmsProtocolParser;
class MyTcpWorker : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpWorker(MyTcpServer * const &p, qintptr socketHandle, QObject *parent = 0);
    virtual ~MyTcpWorker();

signals:

    void sigSendData2TcpSocket(QString data, int id);

    void sigSendData2TcpSocket(const QByteArray &data, int id);

    void sigDisConnectTcpSocket(int id);

    void sigClientDisconnect(int id);

    void sigClientHeartStatus(int id, int nHeartOutTimes);

    void sigRevMessage(int id, int cmdID, QString data);

    void sendDataToServer(int id, const QVariant *value);

public slots:

    void sltRevMessageFromClient();

    void sltClientDisConnect();

    void sltNewClient(int id);

    void sltSendData(int id, QString data);

    void sltSendData(int id, const CmsMessageContent &message);

    void sltDisConnectClient(int id);

protected:

QByteArray createPacket(const CmsMessageContent &message);

    MyTcpSocket             *m_tcpSocket = Q_NULLPTR;
    MyTcpServer             *m_pTcpServer = Q_NULLPTR;
    CmsProtocolParser       *m_Parser = Q_NULLPTR;
    QVector<QByteArray>     m_RecevData;
    qintptr                 m_nID;

};

#endif // HMYTCPWORKER_H
