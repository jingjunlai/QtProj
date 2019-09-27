#include "gpsmodel.h"
#include "module/moduleecg.h"
#include "module/modulespo2.h"
#include "module/moduleresp.h"
#include "module/modulenibp.h"
#include "module/moduleibp.h"
#include "module/moduletemp.h"
#include "module/moduleco2.h"
#include "module/moduleco.h"
#include "mainwindow.h"

GpsModel::GpsModel(QObject *parent)
    : Model(parent)
{

}

GpsModel::~GpsModel()
{
    qDebug()<<"~GpsModel ID = "<<QThread::currentThreadId();
}


void GpsModel::receiveData()//(const QByteArray &data)
{
    static unsigned int uiSize = 0 , uiCnt = 0;
    static unsigned int uiIndex = 0;
    static unsigned int uiHeadCnt = 0;
    static unsigned int uiLen = 0;
    static unsigned char ucBuff[RECEIVE_SIZE] = {0};
    static unsigned char ucCnt = 0;
    //unsigned char *pcData = (unsigned char *)data.data();
    unsigned char *pcData = (unsigned char *)m_Ringbuffer.read().data();
    uiCnt = m_Ringbuffer.read().size();//data.size();
    uiSize += m_Ringbuffer.read().size();//data.size();
    //addPortCnt(uiSize);

    while(uiCnt--)
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
            if(uiLen > RECEIVE_SIZE)
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
                    unpackData(ucBuff, uiLen);
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

void GpsModel::handleData(const QByteArray &data)
{
    m_Ringbuffer.append(data);
}

void GpsModel::handleSocketData(const QByteArray &data)
{
    m_Ringbuffer.append(data);
}

void GpsModel::doWork()
{
    qDebug()<<"GpsModel do wrok ID = "<<QThread::currentThreadId();
    /*新建模块：Ecg*/
    ModuleProperty mp;
    mp.moduleId = MODULE_ID_ECG;
    mp.labelId = LABEL_ECG;
    mp.enable = false;
    mp.protocol = Protocol::Protocol_Cms;
    auto EcgModule = new ModuleEcg(mp);
    m_ModuleList.append(EcgModule);

    /*新建模块：Spo2*/
    mp.moduleId = MODULE_ID_SPO2;
    mp.labelId = LABEL_SPO2;
    mp.enable = false;
    mp.protocol = Protocol::Protocol_Cms;
    auto Spo2Module = new ModuleSpo2(mp);
    m_ModuleList.append(Spo2Module);

    /*新建模块：Resp*/
    mp.moduleId = MODULE_ID_RESP;
    mp.labelId = LABEL_RESP;
    mp.enable = false;
    mp.protocol = Protocol::Protocol_Cms;
    auto RespModule = new ModuleResp(mp);
    m_ModuleList.append(RespModule);

    /*新建模块：Nibp*/
    mp.moduleId = MODULE_ID_NIBP;
    mp.labelId = LABEL_NIBP;
    mp.enable = false;
    mp.protocol = Protocol::Protocol_Cms;
    auto NibpModule = new ModuleNibp(mp);
    m_ModuleList.append(NibpModule);

    /*新建模块：Ibp*/
    for(int i = LABEL_IBP_START; i <= LABEL_IBP_END; i++)
    {
        mp.moduleId = MODULE_ID_IBP;
        mp.labelId = (LabelID)i;
        mp.enable = false;
        mp.protocol = Protocol::Protocol_Cms;
        auto IbpPModule = new ModuleIbp(mp);
        m_ModuleList.append(IbpPModule);
    }

    /*新建模块：Temp*/
    for(int i = LABEL_TEMP_START; i <= LABEL_TEMP_END; i++)
    {
        mp.moduleId = MODULE_ID_TEMP;
        mp.labelId = (LabelID)i;
        mp.enable = false;
        mp.protocol = Protocol::Protocol_Cms;
        auto TempModule = new ModuleTemp(mp);
        m_ModuleList.append(TempModule);
    }

    /*新建模块：Co2*/
    mp.moduleId = MODULE_ID_CO2;
    mp.labelId = LABEL_CO2;
    mp.enable = false;
    mp.protocol = Protocol::Protocol_Cms;
    auto Co2Module = new ModuleCo2(mp);
    m_ModuleList.append(Co2Module);

    /*新建模块：Co*/
    mp.moduleId = MODULE_ID_CO;
    mp.labelId = LABEL_CO;
    mp.enable = false;
    mp.protocol = Protocol::Protocol_Cms;
    auto CoModule = new ModuleCo(mp);
    m_ModuleList.append(CoModule);

    emit enableMainWork(MainWindow::ModelID);
}

void GpsModel::unpackData(unsigned char *pucBuff, unsigned int uiLen)
{
    //Q_UNUSED(pucBuff);
    static int i = 0;
    QByteArray ba((char*)pucBuff, uiLen);
    qDebug() << "data num =" <<QString::number(i++);
    qDebug() << "ucbuff data =" << ba.toHex();
}




