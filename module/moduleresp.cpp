#include "moduleresp.h"
#include "mathtools.h"
#include "net/cmsrespmoduledata.h"
#include <QVariant>
#include <QDebug>
#include <QThread>

RespParam::RespParam(ModuleID module, LabelID label)
    : ModuleParam(module, label)
{
}

RespParam::~RespParam()
{

}

void RespParam::reset()
{
    m_RespRate = INVALID_DATA;
}

QList<ParamPoint> RespParam::toList()
{
    QList<ParamPoint> respParam;

    ParamPoint point;
    point.label = LABEL_RESP;
    point.subid = Sub_Resp;
    point.data = m_RespRate;
    respParam.append(point);

    point.label = LABEL_RESP;
    point.subid = Sub_RespSource;
    point.data = m_RespSource;
    respParam.append(point);

    return respParam;
}

RespStatus::RespStatus(ModuleID module, LabelID label)
    : ModuleStatus(module, label)
{
}

RespStatus::~RespStatus()
{

}

void RespStatus::reset()
{
    //模块私有部分的状态项
    m_LeadsOff = 0;                                  //呼吸脱落标志位 1:呼吸脱落, 0:呼吸不脱落
    m_Apnea = 0;                                     //1:窒息; 0:不窒息;
    m_HeartBeatDisturbSig = 0;                       //心动干扰标志; 0:正常, 1:呼吸受心动干扰;
    m_RespFilterSwitchSig = 0;                       //0:呼吸滤波关;   1:呼吸滤波开;

    m_RespLeadType = Gps::RESP_Lead_RA_LL;
    m_ApneaAlmTime = Gps::RESP_ApneaDelay_20;

    //模块公有状态部分的状态项
    physicalID = 0;
    powerFrequency = 0;
    patientType = 0;
    errorCode = 0;
    moduleState = Module_StateOnline;
    probeState = Probe_StateNormal;
}

RespWave::RespWave(int size, ModuleID module, LabelID label)
    : ModuleWave(size, module, label)
{
}

RespWave::~RespWave()
{
}

QList<WavePoint> RespWave::toList(WaveID id)
{
    QList<WavePoint> respWavePoint;
    if (id != WAVE_RESP)
        return respWavePoint;

    for (int i = 0; i < size(); i++)
    {
        WavePoint pointer;
        pointer.data = (data() + i)->wave;
        pointer.valid = (data() + i)->bValid;

        if (!pointer.valid)
            pointer.data = INVALID_DATA;

        respWavePoint.append(pointer);
    }

    return respWavePoint;
}

void RespWave::reset()
{

}

RespModuleData::RespModuleData(ModuleID module, LabelID label)
    : ModuleData(module, label)
{
    m_pParam = new RespParam(module, label);
    m_pStatus = new RespStatus(module, label);
    m_pWave = new RingBuffer<RespWaveUnit>(100 * 4);
    reset();
}

RespModuleData::~RespModuleData()
{
    delete m_pParam;
    delete m_pStatus;
    delete m_pWave;
}

void RespModuleData::reset()
{
    m_pParam->reset();
    m_pStatus->reset();
}

ModuleResp::ModuleResp(ModuleProperty property, QObject *parent)
    : IModule(property, parent)
{
    m_pRespModuleData = new RespModuleData(property.moduleId, property.labelId);
}

ModuleResp::~ModuleResp()
{
    delete m_pRespModuleData;
}

void ModuleResp::unpackModuleData(const QByteArray &packet)
{
    Q_UNUSED(packet);
}

bool ModuleResp::setConfig(int id, const QVariant &value)
{
    Q_UNUSED(id);
    Q_UNUSED(value);
    return false;
}

bool ModuleResp::getConfig(int id, QVariant &value)
{
    Q_UNUSED(id);
    Q_UNUSED(value);
    return false;
}

void ModuleResp::setData(const QVariant &value)
{
    auto d = value.value<RespDataField>();

    m_pRespModuleData->m_pParam->m_RespRate = (d.respRate == 255? INVALID_DATA : d.respRate);

    m_pRespModuleData->m_pStatus->m_LeadsOff = d.sensorOff;
    m_pRespModuleData->m_pStatus->m_Apnea = d.apnea;
    m_pRespModuleData->m_pStatus->m_RespLeadType = (Gps::RespLeadType)d.calcLead;

    m_modulePacket.ParamPackets++;
    m_modulePacket.WavePackets += d.waves.size();
    if(m_modulePacket.Count++ >= 1)
    {
        m_modulePacket.Count = 0;
        m_pRespModuleData->m_ModuleInfo.packets.ParamPackets = m_modulePacket.ParamPackets;
        m_pRespModuleData->m_ModuleInfo.packets.WavePackets = m_modulePacket.WavePackets;
        m_modulePacket.ParamPackets = 0;
        m_modulePacket.WavePackets = 0;
    }

    RespWaveUnit *pw = new RespWaveUnit[d.waves.size()];

    for(int i = 0; i < d.waves.size(); i++)
    {
        if(d.sensorOff == true || d.respRate == 255)
        {
            pw[i].bValid = false;
            continue;
        }
        pw[i].bValid = true;
//        auto FrameData = d.waves.at(i);
        pw[i].wave = d.waves.at(i).respWave;
    }
    if(d.waves.size() <= m_pRespModuleData->m_pWave->capacity())
    {
        m_pRespModuleData->m_pWave->write(pw, d.waves.size());
    }
    else
    {
        qDebug() << "Resp too much Data!!";
    }
    delete [] pw;

//    qDebug() << "module resp set Data";

}

void ModuleResp::getData(QVariant &value)
{
    value.setValue(m_pRespModuleData);
}
