#include "serialport.h"
#include "iprotocolparser.h"
#include "iserialdatahandler.h"
#include <QSerialPort>
#include <QThread>
#include <QDebug>
#include <QTimerEvent>

SerialPort::SerialPort(const SerialSetting &setting, QObject *parent)
    : ISerialPort(setting, parent)
{
    mReadTimerId = 0;
    mStatisticsTimerId = 0;

    m_pSerialPort = new QSerialPort(this);
    m_pSerialPort->setPortName(setting.port);
    m_pSerialPort->setBaudRate(setting.baudRate);
    m_pSerialPort->setDataBits(setting.dataBits);
    m_pSerialPort->setStopBits(setting.stopBits);
    m_pSerialPort->setParity(setting.parity);
    m_pSerialPort->setFlowControl(QSerialPort::NoFlowControl);

    connect(this, &SerialPort::openSerialPort, this, &SerialPort::onOpenSerialPort, Qt::QueuedConnection);
    connect(this, &SerialPort::closeSerialPort, this, &SerialPort::onCloseSerialPort, Qt::QueuedConnection);
}

SerialPort::~SerialPort()
{
    if(m_pSerialPort == Q_NULLPTR)
        return;

    delete m_pSerialPort;
}

bool SerialPort::open(QIODevice::OpenMode mode)
{
    mOpenMode = mode;
    emit openSerialPort();
    return true;
}

void SerialPort::close()
{
    emit closeSerialPort();
}

qint64 SerialPort::write(const QByteArray &data)
{
    if (isOpen() == false || !mParser)
        return 0;

    QByteArray target = mParser->makePacket(data);

    int ret = m_pSerialPort->write(target.constData(), target.size());
    mStatisticsInfo[0].sendTotalBytes += ret;
    mStatisticsInfo[0].sendPackets++;
    return ret;
}

bool SerialPort::isOpen() const
{
    if(m_pSerialPort == Q_NULLPTR)
        return false;

    return m_pSerialPort->isOpen();
}

void SerialPort::onCloseSerialPort()
{
    if(m_pSerialPort == Q_NULLPTR)
        return;

    m_pSerialPort->close();
    killTimer(mReadTimerId);
    killTimer(mStatisticsTimerId);

    mReadTimerId = 0;
    mStatisticsTimerId = 0;

    mStatisticsInfo[0].init();
    mStatisticsInfo[1].init();
}

bool SerialPort::onOpenSerialPort()
{
    if(isOpen())
        return true;

    auto mode = mOpenMode;
    if (mode != QIODevice::ReadOnly
                && mode != QIODevice::WriteOnly
                && mode != QIODevice::ReadWrite)
        return false;

    mReadTimerId = startTimer(20);
    mStatisticsTimerId = startTimer(1000);


    if(!m_pSerialPort->open(mode))
    {
        qDebug()<<"port open failed!";
        return false;
    }

    return true;
}

void SerialPort::timerEvent(QTimerEvent *event)
{
    if (mReadTimerId == event->timerId())
    {
        unsigned char buf[10240];
        memset(&buf, 0 ,sizeof(buf));
        int len = m_pSerialPort->read((char *)buf, sizeof(buf));
        if (len <= 0)
            return;
        qDebug()<<"BufSize:" <<len <<"bufValue:" <<QByteArray((char*)buf, len);
        mParser->appendData(buf, len);
        mStatisticsInfo[0].receivedTotalBytes += len;

        QVector<QByteArray> packets = mParser->getLegalPackets();
//        if (mDataHandler)
//        {
        qDebug()<<"SerialPort do wrok ID = "<<QThread::currentThreadId();
            ProtocolPacket tmp;
            tmp.protocol = mParser->getProtocolType();
            foreach (auto &value, packets)
            {
                tmp.data = value;
                qDebug() << "handler recv packet = " << tmp.data.toHex();
//                mDataHandler->parseData(tmp);
            }
//        }

        mStatisticsInfo[0].receivedCorrectPackets += packets.size();
        foreach (auto &value, packets)
        {
            mStatisticsInfo[0].receivedCorrectBytes += value.size();
        }
    }
    else if (mStatisticsTimerId == event->timerId())
    {
        mStatisticsInfo[0].receivedErrorPackets = mParser->getErrorPacketCounts();

        mStatisticsInfo[0].receivedBytesPerSecond =
                mStatisticsInfo[0].receivedTotalBytes - mStatisticsInfo[1].receivedTotalBytes;
        mStatisticsInfo[0].receivedCorrectBytesPerSecond =
                mStatisticsInfo[0].receivedCorrectBytes - mStatisticsInfo[1].receivedCorrectBytes;
        mStatisticsInfo[0].receivedErrorPacketsPerSecond =
                mStatisticsInfo[0].receivedErrorPackets - mStatisticsInfo[1].receivedErrorPackets;
        mStatisticsInfo[0].receivedCorrectPacketsPerSecond =
                mStatisticsInfo[0].receivedCorrectPackets - mStatisticsInfo[1].receivedCorrectPackets;
        mStatisticsInfo[0].sendPacketsPerSecond =
                mStatisticsInfo[0].sendPackets - mStatisticsInfo[1].sendPackets;
        mStatisticsInfo[0].sendBytesPerSecond =
                mStatisticsInfo[0].sendTotalBytes - mStatisticsInfo[1].sendTotalBytes;

        mStatisticsInfo[1] = mStatisticsInfo[0];

        if (false)
        {
            // debug statistics infomation
            qDebug() << "serial port statistics infomation\n"
                     << "receive bytes:" << mStatisticsInfo[0].receivedTotalBytes << "\n"
                     << "receive correct bytes:" << mStatisticsInfo[0].receivedCorrectBytes << "\n"
                     << "receive error packets:" << mStatisticsInfo[0].receivedErrorPackets << "\n"
                     << "receive correct packets:" << mStatisticsInfo[0].receivedCorrectPackets << "\n"
                     << "receive bytes 1s:" << mStatisticsInfo[0].receivedBytesPerSecond << "\n"
                     << "receive correct bytes 1s:" << mStatisticsInfo[0].receivedCorrectBytesPerSecond << "\n"
                     << "receive error packets 1s:" << mStatisticsInfo[0].receivedErrorPacketsPerSecond << "\n"
                     << "receive correct packets 1s:" << mStatisticsInfo[0].receivedCorrectPacketsPerSecond << "\n"
                     << "send bytes:" << mStatisticsInfo[0].sendTotalBytes << "\n"
                     << "send packets:" << mStatisticsInfo[0].sendPackets << "\n"
                     << "send bytes 1s:" << mStatisticsInfo[0].sendBytesPerSecond << "\n"
                     << "send packets 1s:" << mStatisticsInfo[0].sendPacketsPerSecond;
        }
    }
}
