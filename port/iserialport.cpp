#include "iserialport.h"


ISerialPort::ISerialPort(const SerialSetting &settings, QObject *parent)
    : QObject(parent)
{
    mSetting = settings;
}

ISerialPort::~ISerialPort()
{

}

void ISerialPort::setParser(IProtocolParser *pParser)
{
    mParser = pParser;
}

void ISerialPort::setSerialSetting(const SerialSetting &setting)
{
    if (isOpen())
        return;
    mSetting = setting;
}

void ISerialPort::setDataHandler(ISerialDataHandler *handler)
{
    mDataHandler = handler;
}

void ISerialPort::start()
{
    open(QIODevice::ReadWrite);
}

DeviceStatisticsInfo ISerialPort::getStatisticsInfo() const
{
    return mStatisticsInfo[0];
}
