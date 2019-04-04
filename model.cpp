#include "model.h"

Model::Model(QObject *parent) : QObject(parent)
{

}

Model::~Model()
{
    qDebug()<<"~Model ID = "<<QThread::currentThreadId();
}

void Model::setPort(MySerialPort * const pThreadPort)
{
    if(pThreadPort != NULL)
    {
        m_pSerialport = pThreadPort;
        connect(m_pSerialport, SIGNAL(sendDataToModel(QByteArray)), this, SLOT(receiveData(QByteArray)), Qt::QueuedConnection);
    }
}

void Model::addPortCnt(unsigned int uiSize)
{
    emit sendPortDataSizeToMain(uiSize);
}




