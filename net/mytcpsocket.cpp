#include "mytcpsocket.h"
#include <QThread>

MyTcpSocket::MyTcpSocket(qintptr socketDescriptor, QObject *parent) :
    QTcpSocket(parent),
    socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
}

MyTcpSocket::~MyTcpSocket()
{
    qDebug() << "~MyTcpSocket ID = " << QThread::currentThreadId();
    this->disconnectFromHost();
}

void MyTcpSocket::sendData(QString data, const int id)
{  
    if(id == socketID)
    {
        QByteArray byteData = data.toUtf8();
        QString strfData = QString("%1").arg(byteData.size(), 8, 10, QChar('0')) + data;
        QByteArray byte = strfData.toUtf8();
        write(byte, byte.size());
    }
}

void MyTcpSocket::sendData(const QByteArray &data, const int id)
{
    if(id == socketID)
    {
        write(data);
    }
}

void MyTcpSocket::disConnectTcp(int id)
{
    if(id == socketID)
    {
        this->disconnectFromHost();
    }
    else if(id == -1)
    {
        this->disconnectFromHost();
        this->deleteLater();
    }
}

