#ifndef HMYTCPSOCKET_H
#define HMYTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(qintptr socketDescriptor, QObject *parent = 0);
    ~MyTcpSocket();

private:
    qintptr socketID;

signals:

public slots:

    void sendData(QString data, const int);

    void sendData(const QByteArray &data, const int id);

    void disConnectTcp(int id);
};

#endif // HMYTCPSOCKET_H
