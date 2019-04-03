#include "gpsmodel.h"

GpsModel::GpsModel()
{

}

GpsModel::~GpsModel()
{
    qDebug()<<"~MySerialPort ID = "<<QThread::currentThreadId();
}


void GpsModel::handleData(const QByteArray &Data)
{
    Q_UNUSED(Data);
}

void GpsModel::doWork()
{
    qDebug()<<"GpsModel do wrok ID = "<<QThread::currentThreadId();
}

