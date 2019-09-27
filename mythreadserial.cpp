#include "mythreadserial.h"
#include "mainwindow.h"

MySerialPort::MySerialPort(QObject *parent) : QObject(parent)
{

}

MySerialPort::MySerialPort(const QString Port, const int &iBuadRate, QObject *parent) : QObject(parent)
{
    m_Port = Port;
    m_iBuadRate = iBuadRate;
    //m_pRingbuffer = new QRingBuffer;
}

MySerialPort::~MySerialPort()
{
    qDebug()<<"~MySerialPort ID = "<<QThread::currentThreadId();
    disconnect(m_pSerial,SIGNAL(readyRead()),this,SLOT(readSerialData()));
    m_pSerial->close();
    m_pSerial->deleteLater();
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
    if(!m_pSerial->bytesAvailable())
        return;
    QByteArray data = m_pSerial->readAll();
    //qDebug() << QStringLiteral("data received(收到的数据):") << data.toHex();
    //QThread::usleep(10);
    //m_pRingbuffer->append(data);
    //testReceiveData(data);
    emit sendDataToModel(data);
}

void MySerialPort::doWork()
{
    qDebug()<<"SerialPort do wrok ID = "<<QThread::currentThreadId();
    m_pSerial = new QSerialPort(this);
    m_pSerial->setPortName(m_Port);
    m_pSerial->setBaudRate(m_iBuadRate);
    m_pSerial->setDataBits(QSerialPort::Data8);
    m_pSerial->setStopBits(QSerialPort::OneStop);
    m_pSerial->setParity(QSerialPort::NoParity);
    m_pSerial->setFlowControl(QSerialPort::NoFlowControl);
    if(!m_pSerial->open(QIODevice::ReadWrite))
    {
        qDebug()<<"port open failed!";
        return;
    }
    connect(m_pSerial,SIGNAL(readyRead()),this,SLOT(readSerialData()),Qt::QueuedConnection);
    emit enableMainWork(MainWindow::SerialID);
//    m_pTimer = new QTimer(this);
//    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(testReceiveData()));
//    m_pTimer->start(10);
}

void MySerialPort::testReceiveData()
{
//    static unsigned int uiSize = 0;
    static unsigned int uiIndex = 0;
    static unsigned int uiHeadCnt = 0;
    static unsigned int uiLen = 0;
    static unsigned char ucBuff[250] = {0};
    static unsigned char ucCnt = 0;
    //unsigned char *pcData = (unsigned char *)Data.data();
    //unsigned char *pcData = (unsigned char *)m_pRingbuffer->read().data();
    volatile char pccData[500] = {0};
    m_Mutex.lock();
    //m_pRingbuffer.read((char *)pccData, 500);
    volatile unsigned char *pcData = (unsigned char *)pccData;
    m_Mutex.unlock();
//    uiSize += data.size();
//    addPortCnt(uiSize);

    while(*pcData)
    {
        switch(uiHeadCnt)
        {
        case 0:         /*包头0xAA*/
            if(*pcData == 0xAA)
            {
                uiHeadCnt++;
            }
            break;
        case 1:         /*包头0xAB*/
            if(*pcData == 0xAB)
            {
                uiHeadCnt++;
            }
            else
            {
                uiHeadCnt = 0;
            }
            break;
        case 2:         /*通道号 主通道bit7~4 子通道bit3~0*/
            uiIndex = 0;
            //ucBuff[uiIndex++] = *pcData;

            ucBuff[uiIndex++] = ucCnt++;
            uiHeadCnt++;
            break;
        case 3:         /*长度*/
            uiLen = *pcData;
            if(uiLen > 250)
            {
                uiHeadCnt = 0;
                qDebug() << "port data len > RECEIVE_SIZE";
            }
            else
            {
                ucBuff[uiIndex++] = *pcData;
            }
            uiHeadCnt++;
            break;
        default:        /*主体数据*/
            if(uiIndex < uiLen + 1)
            {
                ucBuff[uiIndex++] = *pcData;
            }
            else if(uiIndex == uiLen + 1)
            {
                unsigned char ucSum = 0;
                for(uint j = 0; j < uiLen; j++)
                {
                    ucSum += ucBuff[j + 1];
                }
                if(ucSum == *pcData)
                {
                    uiHeadCnt = 0;
                    ucBuff[uiLen + 1] = '\0';
                    //unpackData(ucBuff);
                }
                else
                {
                    uiHeadCnt = 0;
                    qDebug() << "port data sum error!!";
                }
            }
            else
            {
                uiHeadCnt = 0;
                qDebug() << "port data uiIndex > uiLen + 1";
            }
            break;
        }
        pcData++;
    }
}

