#include "model.h"

Model::Model(QObject *parent) : QObject(parent)
{

}

Model::~Model()
{
    qDebug()<<"~Model ID = "<<QThread::currentThreadId();
    disconnect(m_pSerialport, SIGNAL(sendDataToModel(QByteArray)), this, SLOT(handleData(QByteArray)));
}

void Model::setPort(MySerialPort * const pThreadPort)
{
    if(pThreadPort != NULL)
    {
        m_pSerialport = pThreadPort;
        connect(m_pSerialport, SIGNAL(sendDataToModel(QByteArray)), this, SLOT(handleData(QByteArray)), Qt::QueuedConnection);
    }
}


