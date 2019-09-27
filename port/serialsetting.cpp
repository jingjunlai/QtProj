#include "serialsetting.h"

SerialSetting::SerialSetting(QString port,
                             qint32 baudRate,
                             QSerialPort::DataBits dataBits,
                             QSerialPort::Parity parity,
                             QSerialPort::StopBits stopBits)
    : port(port)
    , baudRate(baudRate)
    , dataBits(dataBits)
    , parity(parity)
    , stopBits(stopBits)
    , separation(false)
{

}

SerialSetting::SerialSetting()
{

}

SerialSetting::~SerialSetting()
{

}

void SerialSetting::setPort(QString port)
{
    this->port = port;
}

void SerialSetting::setBaudRate(qint32 baudRate)
{
    this->baudRate = baudRate;
}

void SerialSetting::setDataBits(QSerialPort::DataBits dataBits)
{
    this->dataBits = dataBits;
}

void SerialSetting::setParity(QSerialPort::Parity parity)
{
    this->parity = parity;
}

void SerialSetting::setStopBits(QSerialPort::StopBits stopBits)
{
    this->stopBits = stopBits;
}

void SerialSetting::setSeparation(bool separation)
{
    this->separation = separation;
}
