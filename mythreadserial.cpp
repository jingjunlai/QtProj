#include "mythreadserial.h"

MyThreadSerial::MyThreadSerial(const QString &port, const int &BuadRate, QObject *parent) : QObject(parent)
{
    m_Thread = new QThread();
    m_Serial = new QSerialPort(this);
    m_Serial->setPortName(port);
    m_Serial->setBaudRate(BuadRate);
    m_Serial->setDataBits(QSerialPort::Data8);
    m_Serial->setStopBits(QSerialPort::OneStop);
    m_Serial->setParity(QSerialPort::NoParity);
    m_Serial->setFlowControl(QSerialPort::NoFlowControl);
    if(!m_Serial->open(QIODevice::ReadWrite))
    {
        QMessageBox::about(NULL, "提示", "串口打开失败");
        return;
    }
    connect(m_Serial,SIGNAL(readyRead()),this,SLOT(readSerialData()),Qt::QueuedConnection);
    this->moveToThread(m_Thread);
    m_Serial->moveToThread(m_Thread);
    m_Thread->start();
}

MyThreadSerial::~MyThreadSerial()
{
    m_Serial->close();
    m_Serial->deleteLater();
    m_Thread->quit();
    m_Thread->wait();
    m_Thread->deleteLater();
}

void MyThreadSerial::readSerialData()
{
    QByteArray data = m_Serial->readAll();
//    qDebug() << QStringLiteral("data received(收到的数据):") << data;
//    qDebug() << "handing thread is:" << QThread::currentThreadId();
    m_Thread->usleep(10);
    emit sendDataToMain(data);
}

