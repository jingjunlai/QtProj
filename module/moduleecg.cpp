#include "moduleecg.h"
#include "mathtools.h"
#include "net/cmsecgmoduledata.h"
#include "net/cmsecgconfigdata.h"
#include "net/cmsarrdata.h"
#include <QVariant>
#include <QDebug>
#include <QThread>

ECGParam::ECGParam(ModuleID module, LabelID label)
    : ModuleParam(module, label)
{
}

ECGParam::~ECGParam()
{

}

void ECGParam::reset()
{
    hr = INVALID_DATA;
    hrConfidenceLevel = 0;
    arrCode = 0;
//    for (int i = 0; i < 12; i++)
//    {
//        stValue[i] = INVALID_DATA;
//    }
    pvcMin = INVALID_DATA;
    pvcCnt = INVALID_DATA;
//    memset(sqi, 0, sizeof(sqi));
//    memset(arrWave, 0, sizeof(arrWave));        // 主要导联的数据，长度8秒，从心律失常发生往前后各四秒
    stUpdate = 1;                               // 1=更新，2=ST无法更新，0=中间状态
    newArrEvent = INVALID_DATA; 		        // 新的心律失常事件标记
    qtcValue = INVALID_DATA;
    baseQtcValue = INVALID_DATA;
    autoJPos = 0;                               //0 算法定义无效值
    autoISOPos = 0;                             //0 算法定义无效值
    bFindPace = false;
    slead1 = 255;
    slead2 = 255;                               // 自动选择了哪两个导联。 255=无效值
    //memset(&measureOut, 0, sizeof(EcgDiagLibOutput));
//    stBase.reset();
//    qtBase.reset();
}

QList<ParamPoint> ECGParam::toList()
{
    QList<ParamPoint> ret;

    ParamPoint point;
    point.label = LABEL_ECG;
    point.subid = Sub_Hr;
    point.data = hr;
    ret.append(point);

    return ret;
}

//****************ECGStatus class*******************//
ECGStatus::ECGStatus(ModuleID module, LabelID label)
    : ModuleStatus(module, label)
{

}


//**************************ECGWave******************************//
ECGWave::ECGWave(int size, ModuleID module, LabelID label)
    : ModuleWave(size, module, label)
{

}

ECGWave::~ECGWave()
{

}

QList<WavePoint> ECGWave::toList(WaveID wave)
{
    QList<WavePoint> ret;
    uint waveID = wave - WAVE_ECG_START;
    if (waveID >= 12)
        return ret;
    for (int i = 0; i < size(); i++)
    {
        WavePoint point;
        point.data = (data() + i)->ecg[waveID];
        point.extend = GET_BIT((data() + i)->paceFlag, waveID);     // 起搏标志位
        point.valid = GET_BIT((data() + i)->wvFlag, waveID);        //有效状态
        ret.append(point);
    }
    return ret;
}

EcgModuleData::EcgModuleData(ModuleID module, LabelID label)
    : ModuleData(module, label)
{
    m_pParam = new ECGParam(module, label);
    m_pStatus = new ECGStatus(module, label);
    m_pWave = new RingBuffer<ECGWaveUnit>(1000 * 6);
    reset();
}

EcgModuleData::~EcgModuleData()
{
    delete m_pParam;
    delete m_pStatus;
    delete m_pWave;
}

void EcgModuleData::reset()
{
    m_pParam->reset();
    m_pStatus->reset();
}

ModuleEcg::ModuleEcg(ModuleProperty property, QObject *parent)
    : IModule(property, parent)
{
    m_pEcgModuleData = new EcgModuleData;
}

ModuleEcg::~ModuleEcg()
{
    delete m_pEcgModuleData;
}

void ModuleEcg::unpackModuleData(const QByteArray &packet)
{
    Q_UNUSED(packet);
}

bool ModuleEcg::setConfig(int id, const QVariant &value)
{
    Q_UNUSED(id);
    auto d = value.value<EcgConfigField>();

    m_pEcgModuleData->m_pStatus->workMode = (Gps::EcgMode)d.workMode;

    return true;
}

bool ModuleEcg::getConfig(int id, QVariant &value)
{
    Q_UNUSED(id);
    Q_UNUSED(value);
    return false;
}

void ModuleEcg::setData(const QVariant &value)
{
    auto d = value.value<EcgDataField>();
//    qDebug() << "module ecg set Data";

    m_pEcgModuleData->m_pParam->hr = d.hr;
    m_pEcgModuleData->m_pStatus->leadType = d.leadType;
    m_pEcgModuleData->m_pStatus->calLead = d.calcLeadIndex;

    m_pEcgModuleData->m_pStatus->sensorOff = false;
    for(int i = 0; i < Gps::EcgElecMax; i++)
    {
        if((m_pEcgModuleData->m_pStatus->electrodeOff[i] = d.electrodeOff[i]) == true)
            m_pEcgModuleData->m_pStatus->sensorOff = true;
    }

    for(int i = 0; i < Gps::ECG_LEAD_MAX; i++)
    {
        m_pEcgModuleData->m_pStatus->electrodePolar[i] = d.polarization[i];
    }

//    memset(m_pEcgModuleData->m_pStatus->leadVaild, false, sizeof(m_pEcgModuleData->m_pStatus->leadVaild));

//    m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_I] = !(d.electrodeOff[CMS_EcgElecRa] || d.electrodeOff[CMS_EcgElecLa]);
//    m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_II] = !(d.electrodeOff[CMS_EcgElecRa] || d.electrodeOff[CMS_EcgElecLl]);

//    switch(d.leadType)
//    {
//        case CMS_EcgType3:
//        case CMS_EcgType5:
//            m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_V_1] = !d.electrodeOff[CMS_EcgElecC1];
//            break;
//        case CMS_EcgType6:
//            m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_V_1] = !d.electrodeOff[CMS_EcgElecC1];
//            m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_V_2] = !d.electrodeOff[CMS_EcgElecC2];
//            break;
//        case CMS_EcgType12:
//            m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_V_1] = !d.electrodeOff[CMS_EcgElecC1];
//            m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_V_2] = !d.electrodeOff[CMS_EcgElecC2];
//            m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_V_3] = !d.electrodeOff[CMS_EcgElecC3];
//            m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_V_4] = !d.electrodeOff[CMS_EcgElecC4];
//            m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_V_5] = !d.electrodeOff[CMS_EcgElecC5];
//            m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_V_6] = !d.electrodeOff[CMS_EcgElecC6];
//            m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_III] = (m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_I] && m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_II]);
//            m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_AVR] = (m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_I] && m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_II]);
//            m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_AVL] = (m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_I] && m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_II]);
//            m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_AVF] = (m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_I] && m_pEcgModuleData->m_pStatus->leadVaild[Gps::ECG_LEAD_II]);
//            break;
//        default:
//            break;
//    }



    m_modulePacket.ParamPackets++;
    m_modulePacket.WavePackets += d.waves.size();
    if(m_modulePacket.Count++ >= 1)
    {
        m_modulePacket.Count = 0;
        m_pEcgModuleData->m_ModuleInfo.packets.ParamPackets = m_modulePacket.ParamPackets;
        m_pEcgModuleData->m_ModuleInfo.packets.WavePackets = m_modulePacket.WavePackets;
        m_modulePacket.ParamPackets = 0;
        m_modulePacket.WavePackets = 0;
    }
    ECGWaveUnit *pw = new ECGWaveUnit[d.waves.size()];

    for(int i = 0; i < d.waves.size(); i++)
    {
        auto FrameData = d.waves.at(i);

        pw[i].rFlag = FrameData.rwave;

        pw[i].vaild[Gps::ECG_LEAD_I] = !(d.electrodeOff[CMS_EcgElecRa] || d.electrodeOff[CMS_EcgElecLa]);
        pw[i].vaild[Gps::ECG_LEAD_II] = !(d.electrodeOff[CMS_EcgElecRa] || d.electrodeOff[CMS_EcgElecLl]);

        switch(d.leadType)
        {
            case CMS_EcgType3:
            case CMS_EcgType5:
                pw[i].vaild[Gps::ECG_LEAD_V_1] = !d.electrodeOff[CMS_EcgElecC1];
                break;
            case CMS_EcgType6:
                pw[i].vaild[Gps::ECG_LEAD_V_1] = !d.electrodeOff[CMS_EcgElecC1];
                pw[i].vaild[Gps::ECG_LEAD_V_2] = !d.electrodeOff[CMS_EcgElecC2];
                break;
            case CMS_EcgType12:
                pw[i].vaild[Gps::ECG_LEAD_V_1] = !d.electrodeOff[CMS_EcgElecC1];
                pw[i].vaild[Gps::ECG_LEAD_V_2] = !d.electrodeOff[CMS_EcgElecC2];
                pw[i].vaild[Gps::ECG_LEAD_V_3] = !d.electrodeOff[CMS_EcgElecC3];
                pw[i].vaild[Gps::ECG_LEAD_V_4] = !d.electrodeOff[CMS_EcgElecC4];
                pw[i].vaild[Gps::ECG_LEAD_V_5] = !d.electrodeOff[CMS_EcgElecC5];
                pw[i].vaild[Gps::ECG_LEAD_V_6] = !d.electrodeOff[CMS_EcgElecC6];
                pw[i].vaild[Gps::ECG_LEAD_III] = (pw[i].vaild[Gps::ECG_LEAD_I] && pw[i].vaild[Gps::ECG_LEAD_II]);
                pw[i].vaild[Gps::ECG_LEAD_AVR] = (pw[i].vaild[Gps::ECG_LEAD_I] && pw[i].vaild[Gps::ECG_LEAD_II]);
                pw[i].vaild[Gps::ECG_LEAD_AVL] = (pw[i].vaild[Gps::ECG_LEAD_I] && pw[i].vaild[Gps::ECG_LEAD_II]);
                pw[i].vaild[Gps::ECG_LEAD_AVF] = (pw[i].vaild[Gps::ECG_LEAD_I] && pw[i].vaild[Gps::ECG_LEAD_II]);
                break;
            default:
                break;
        }

        pw[i].ecg[Gps::ECG_LEAD_I] = FrameData.wave[0];
        pw[i].ecg[Gps::ECG_LEAD_II] = FrameData.wave[1];
        pw[i].ecg[Gps::ECG_LEAD_V_1] = FrameData.wave[2];
        pw[i].ecg[Gps::ECG_LEAD_V_2] = FrameData.wave[3];
        pw[i].ecg[Gps::ECG_LEAD_V_3] = FrameData.wave[4];
        pw[i].ecg[Gps::ECG_LEAD_V_4] = FrameData.wave[5];
        pw[i].ecg[Gps::ECG_LEAD_V_5] = FrameData.wave[6];
        pw[i].ecg[Gps::ECG_LEAD_V_6] = FrameData.wave[7];
        pw[i].ecg[Gps::ECG_LEAD_III] = pw[i].ecg[Gps::ECG_LEAD_II] - pw[i].ecg[Gps::ECG_LEAD_I];
        pw[i].ecg[Gps::ECG_LEAD_AVR] = -(pw[i].ecg[Gps::ECG_LEAD_I] + pw[i].ecg[Gps::ECG_LEAD_II]) / 2;
        pw[i].ecg[Gps::ECG_LEAD_AVL] = (pw[i].ecg[Gps::ECG_LEAD_I] - pw[i].ecg[Gps::ECG_LEAD_III]) / 2;
        pw[i].ecg[Gps::ECG_LEAD_AVF] = (pw[i].ecg[Gps::ECG_LEAD_II] + pw[i].ecg[Gps::ECG_LEAD_III]) / 2;
    }
    if(d.waves.size() <= m_pEcgModuleData->m_pWave->capacity())
    {
        m_pEcgModuleData->m_pWave->write(pw, d.waves.size());
    }
    else
    {
        qDebug() << "too much Data!!";
    }
//    qDebug() << "ecg module set data finish!";
    delete [] pw;
}

void ModuleEcg::setArrData(const QVariant &value)
{
    auto d = value.value<ArrDataField>();

    m_pEcgModuleData->m_pParam->pvcCnt = d.Pvcs;
}

void ModuleEcg::getData(QVariant &value)
{
    value.setValue(m_pEcgModuleData);
}





