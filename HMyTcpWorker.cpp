#include "HMyTcpWorker.h"
#include <QDebug>
#include <QXmlStreamReader>

HMyTcpWorker::HMyTcpWorker(qintptr socketHandle, QObject *parent)
    : QObject(parent),
      m_nID(socketHandle),
      m_tcpSocket(NULL)
{

}

void HMyTcpWorker::sltNewClient(int id)
{
    if(m_tcpSocket == NULL)
    {
        m_nID = id;

        m_tcpSocket = new HMyTcpSocket(m_nID);

        connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(sltRevMessageFromClient()));
        connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(sltClientDisConnect()));
        connect(this, SIGNAL(sigDisConnectTcpSocket(int)), m_tcpSocket, SLOT(disConnectTcp(int)));
        connect(this, SIGNAL(sigSendData2TcpSocket(QString,int)), m_tcpSocket, SLOT(sendData(QString,int)));

        qDebug() << "new Client" << m_nID;
    }
}

void HMyTcpWorker::sltRevMessageFromClient()
{
    HMyTcpSocket* tcpSocket = (HMyTcpSocket*)QObject::sender();
    if(tcpSocket != m_tcpSocket)
    {
        qDebug() << "unKnown Data";
        return;
    }

    if(tcpSocket->bytesAvailable() <= 0)
    {
        qDebug() << "no data";
        return;
    }

    long lengthHeader = QString(tcpSocket->read(8)).toLong();

    if(lengthHeader <= 0)
    {
        qDebug() << "no data 2";
        return;
    }

    emit sigClientHeartStatus(m_nID, 0);

    QByteArray pByte;
    pByte = tcpSocket->read(lengthHeader);

    //解析数据（这部分可以根据自己定义的通信协议及通信的数据结构进行解析
    //我这里采用的是xml的数据结构
    int nIndex = -1;//命令码
    QString strData;
    QXmlStreamReader reader(pByte);
    while(!reader.atEnd())
    {
        if(reader.isStartElement())
        {
            if(reader.name().compare("MCODE", Qt::CaseInsensitive) == 0)
            {
                nIndex = reader.readElementText().remove("0X").toInt();
            }

            if(reader.name().compare("DATA", Qt::CaseInsensitive) == 0)
            {
                strData = reader.readElementText();
            }
        }
        reader.readNext();
    }

    switch (nIndex)
    {
    case 1://心跳包
    {
        qDebug()<<QString::fromLocal8Bit("收到心跳包") << m_nID;
        QString strHeart;
        strHeart = QString("<HEAD><MCODE>%1</MCODE><DATA>heart</DATA></HEAD>").arg("0X01");
        qDebug()<<QString::fromLocal8Bit("回复心跳包");
        emit sigSendData2TcpSocket(strHeart, m_nID);
    }
        break;
    default:
        break;
    }

    if(nIndex != -1)
    {
        emit sigRevMessage(m_nID, nIndex, strData);
    }
}

void HMyTcpWorker::sltClientDisConnect()
{
    qDebug() << "client is disconnected" << m_nID;
    emit sigClientDisconnect(m_nID);
}

void HMyTcpWorker::sltDisConnectClient(int id)
{
    if(m_tcpSocket)
    {
        if(id == m_nID || id == -1)
        {
            emit sigDisConnectTcpSocket(m_nID);
        }
    }
}

void HMyTcpWorker::sltSendData(int id, QString data)
{
    if(m_tcpSocket)
    {
        if(id == m_nID)
        {
            emit sigSendData2TcpSocket(data, m_nID);
        }
    }
}
