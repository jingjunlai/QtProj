#include "HMyTcpSocket.h"

HMyTcpSocket::HMyTcpSocket(qintptr socketDescriptor, QObject *parent) :
    QTcpSocket(parent),
    socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
}

HMyTcpSocket::~HMyTcpSocket()
{
    this->disconnectFromHost();
    this->deleteLater();
}

void HMyTcpSocket::sendData(QString data, const int id)
{
    //����Ƿ������жϺã�ֱ�ӵ���write����ֿ��̵߳��������Է������ù㲥��ÿ�������ж��Ƿ����Լ�Ҫ���͵���Ϣ��
    if(id == socketID)//�ж��Ƿ��Ǵ�socket����Ϣ
    {
        QByteArray byteData = data.toUtf8();
        QString strfData = QString("%1").arg(byteData.size(), 8, 10, QChar('0')) + data;
        QByteArray byte = strfData.toUtf8();
        write(byte, byte.size());
    }
}

void HMyTcpSocket::disConnectTcp(int id)
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

