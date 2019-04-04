#include "gpsmodel.h"

GpsModel::GpsModel()
{

}

GpsModel::~GpsModel()
{
    qDebug()<<"~GpsModel ID = "<<QThread::currentThreadId();
}


void GpsModel::receiveData(const QByteArray &data)
{
    //Q_UNUSED(data);
    //qDebug() << QStringLiteral("data received(收到的数据):") << data.toHex();
    //QThread::usleep(10);

    static unsigned int uiSize = 0;
    uiSize = data.size();

    static unsigned int uiIndex = 0;
    static unsigned int uiHeadCnt = 0;
    static unsigned int uiLen = 0;
    static unsigned char ucBuff[RECEIVE_SIZE] = {0};

    unsigned char *pcData = (unsigned char *)data.data();
    while(*pcData)
    {
        if(uiHeadCnt == 0)
        {
            if(*pcData == 0xAA)
            {
                uiHeadCnt++;
            }
        }
        else if(uiHeadCnt == 1)
        {
            if(*pcData == 0xAB)
            {
                uiHeadCnt++;
            }
            else
            {
                uiHeadCnt = 0;
            }
        }
        else if(uiHeadCnt == 2)
        {
            uiIndex = 0;
            uiLen = *pcData;
            if(uiLen > RECEIVE_SIZE)
            {
                uiHeadCnt = 0;
            }
            else
            {
                ucBuff[uiIndex++] = *pcData;
            }
            uiHeadCnt++;
        }
        else
        {
            if(uiIndex < uiLen + 1)
            {
                ucBuff[uiIndex++] = *pcData;
            }
            else if(uiIndex == uiLen + 1)
            {
                unsigned char ucSum = 0;
                unsigned int j = 0;
                for(; j < uiLen; j++)
                {
                    ucSum +=ucBuff[j];
                }
                if(ucSum == ucBuff[j])
                {
                    uiHeadCnt = 0;
                    unpackData(ucBuff);
                }
                else
                {
                    uiHeadCnt = 0;
                }
            }
            else
            {
                uiHeadCnt = 0;
            }
        }
        pcData++;
    }
}

void GpsModel::doWork()
{
    qDebug()<<"GpsModel do wrok ID = "<<QThread::currentThreadId();
    if(m_pSerialport != NULL)
        connect(m_pSerialport, SIGNAL(sendDataToModel(QByteArray)), this, SLOT(receiveData(QByteArray)), Qt::QueuedConnection);
}

void GpsModel::unpackData(unsigned char *pucBuff)
{
    //Q_UNUSED(pucBuff);
    QByteArray ba((char*)pucBuff);
    qDebug() << "ucbuff data =" << ba.toHex();

}

