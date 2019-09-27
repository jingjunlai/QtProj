#ifndef HMYTCPSERVER_H
#define HMYTCPSERVER_H

#include "mytcpworker.h"
#include "module/moduleid.h"
#include "model/model.h"

#include <QObject>
#include <QTcpServer>
#include <QThread>
#include <QTimer>
#include <QMutex>

#define HTCPCLIENT_HEART_TIMEOUT_COUNT 3

class CmsCommandHandler;
class CmsArrContentHandler;
class CmsContentHandler;
class CmsMessageContent;
class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:

    explicit MyTcpServer(int iPort, QObject *parent = 0);

    virtual ~MyTcpServer();

    bool    startServer(int port);

    void setModelPoint(Model * const &pModel);

protected:

    void incomingConnection(qintptr handle);

signals:

    void sigNewClient(int id);

    void sigSendData(int id, QString data);

    void sigSendData(int id, const CmsMessageContent &message);

    void sigDisconnectClient(int id);

    void sendDataToModel(QByteArray);

    void enableMainWork(int id);

public slots:

    void sltCheckClientTimer();

    void sltClientHeartStatus(int id, int nHeartOutTimes);

    void sltClientDisconnect(int id);

    void sltRecMessage(int id, int cmdID, QString data);

    void onReadMessage(int Id, const QVariant *value);

    void doWork();

    void transmitConfigData(int id, const QSharedPointer<ICmsData> data);

    void transmitCommand(int id, const QSharedPointer<ICmsData> data);

protected slots:

    void sendHeartBeat();

protected:

    void handlePacket(const CmsMessageContent &content);

    Model                                   *m_pModel = Q_NULLPTR;
    int                                     m_iPort;
    QMap<int, QThread *>                    m_mapThread;
    QMap<int, MyTcpWorker *>                m_mapWorker;
    QMap<int, int>                          m_mapClientHeart;
    QMap<LabelID, CmsContentHandler*>       mParamHandler;
    CmsArrContentHandler                    *m_pArrHandler = Q_NULLPTR;
    CmsCommandHandler                       *m_pCommandHandler = Q_NULLPTR;
    QTimer*                                 m_checkClientTimer;
    QTimer                                  *m_pHeartTimer;
    QMutex                                  m_Mutex;

    int                                     m_iId = 0;  //暂时固定为CMS客户端单个的ID
};

#endif // HMYTCPSERVER_H
