#include "mythreadserial.h"

MySerialPort::MySerialPort(QObject *parent) : QObject(parent)
{

}

MySerialPort::MySerialPort(const QString Port, const int &iBuadRate, QObject *parent) : QObject(parent)
{
    m_Port = Port;
    m_iBuadRate = iBuadRate;
}

MySerialPort::~MySerialPort()
{
    qDebug()<<"~MySerialPort ID = "<<QThread::currentThreadId();
    disconnect(m_Serial,SIGNAL(readyRead()),this,SLOT(readSerialData()));
    m_Serial->close();
    m_Serial->deleteLater();
}

void MySerialPort::detectPort(QStringList *slt)
{
    /*检测串口*/
    foreach (const QSerialPortInfo &Qspinfo, QSerialPortInfo::availablePorts())
    {
        slt->append(Qspinfo.portName());
    }
}

void MySerialPort::readSerialData()
{
    QByteArray data = m_Serial->readAll();
    //qDebug() << QStringLiteral("data received(收到的数据):") << data.toHex();
    //QThread::usleep(10);
    emit sendDataToModel(data);
}

void MySerialPort::doWork()
{
    qDebug()<<"SerialPort do wrok ID = "<<QThread::currentThreadId();
    m_Serial = new QSerialPort(this);
    m_Serial->setPortName(m_Port);
    m_Serial->setBaudRate(m_iBuadRate);
    m_Serial->setDataBits(QSerialPort::Data8);
    m_Serial->setStopBits(QSerialPort::OneStop);
    m_Serial->setParity(QSerialPort::NoParity);
    m_Serial->setFlowControl(QSerialPort::NoFlowControl);
    if(!m_Serial->open(QIODevice::ReadWrite))
    {
        qDebug()<<"port open failed!";
        return;
    }
    connect(m_Serial,SIGNAL(readyRead()),this,SLOT(readSerialData()),Qt::QueuedConnection);
    emit enableMainWork();
}

