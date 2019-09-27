#include "moduleibp.h"
#include "mathtools.h"
#include "net/cmsibpmoduledata.h"
#include <QVariant>
#include <QDebug>
#include <QThread>

IbpParam::IbpParam(ModuleID module, LabelID label)
    : ModuleParam(module, label)
{

}

IbpParam::~IbpParam()
{

}

void IbpParam::reset()
{
    sys = INVALID_DATA;
    dia = INVALID_DATA;
    mean = INVALID_DATA;
    ppv = INVALID_DATA;
    pr = INVALID_DATA;
}

QList<ParamPoint> IbpParam::toList()
{
    QList<ParamPoint> ret;

    ParamPoint point;
    point.label = labelId();
    point.subid = Sub_IbpSys;
    point.data = sys;
    ret.append(point);

    point.label = labelId();
    point.subid = Sub_IbpDia;
    point.data = dia;
    ret.append(point);

    point.label = labelId();
    point.subid = Sub_IbpMean;
    point.data = mean;
    ret.append(point);
    return ret;
}

IbpStatus::IbpStatus(ModuleID module, LabelID label)
    : ModuleStatus(module, label)
{

}


IbpStatus::~IbpStatus()
{

}

void IbpStatus::reset()
{
    sensorOff = 0;
    catheterOff = 0;
    zeroState = 0;
    calibrationState = 0;
    filterState = 0;
    averageTime = 0;

    powerFrequency = 0;
    patientType = 0;
    labelName = 0;
    errorCode = 0;
    physicalID = 0;
    moduleState = Module_StateOnline;
    probeState = Probe_StateNormal;
    zeroSucceedTimes.clear();
    calibrateSucceedTimes.clear();
}

bool IbpStatus::hasError() const
{
    if (sensorOff ==  1 || calibrationState == 1 || zeroState == 1 || errorCode != 0 )
        return true;    //参数无效

//    QVariant value(0);
//    config->getUserConfig(Gps::OperatorConfigID::IBP_ParamValiditySwitch, value);

//    if (value.toInt() == Gps::On)
//    {
//        return false;    //参数有效
//    }
    return (zeroState != 0) || (calibrationState != 0);
}

IbpWave::IbpWave(int size, ModuleID module, LabelID label)
    : ModuleWave(size, module, label)
{
}

IbpWave::~IbpWave()
{
}

void IbpWave::reset()
{

}

QList<WavePoint> IbpWave::toList(WaveID wave)
{
    QList<WavePoint> ret;
    if (wave < WAVE_IBP_START || wave > WAVE_IBP_END)
        return ret;
    for (int i = 0; i < size(); i++)
    {
        WavePoint point;
        point.data = (data() + i)->wave;
        point.valid = true;
        ret.append(point);
    }
    return ret;

}

IbpModuleData::IbpModuleData(ModuleID module, LabelID label)
    : ModuleData(module, label)
{
    m_pParam = new IbpParam(module, label);
    m_pStatus = new IbpStatus(module, label);
    m_pWave = new RingBuffer<IbpWaveUnit>(100 * 6);
    reset();
}

IbpModuleData::~IbpModuleData()
{
    delete m_pParam;
    delete m_pStatus;
    delete m_pWave;
}

void IbpModuleData::reset()
{
    m_pParam->reset();
    m_pStatus->reset();
}

ModuleIbp::ModuleIbp(ModuleProperty property, QObject *parent)
    : IModule(property, parent)
{
    m_pIbpModuleData = new IbpModuleData(property.moduleId, property.labelId);
}

ModuleIbp::~ModuleIbp()
{
    delete m_pIbpModuleData;
}

void ModuleIbp::unpackModuleData(const QByteArray &packet)
{
    Q_UNUSED(packet);
}

bool ModuleIbp::setConfig(int id, const QVariant &value)
{
    Q_UNUSED(id);
    Q_UNUSED(value);
    return false;
}

bool ModuleIbp::getConfig(int id, QVariant &value)
{
    Q_UNUSED(id);
    Q_UNUSED(value);
    return false;
}

void ModuleIbp::setData(const QVariant &value)
{
    auto d = value.value<IbpDataField>();

    m_pIbpModuleData->m_pParam->sys = (d.sys == -10001? INVALID_DATA : d.sys);
    m_pIbpModuleData->m_pParam->dia = (d.dia == -10001? INVALID_DATA : d.dia);
    m_pIbpModuleData->m_pParam->mean = (d.mean == -10001? INVALID_DATA : d.mean);
    m_pIbpModuleData->m_pParam->pr = d.pulseRate;
    m_pIbpModuleData->m_pParam->ppv = d.ppv;

//    m_pIbpModuleData->m_pStatus->sensorOff = d.sensorOff;
    m_pIbpModuleData->m_pStatus->catheterOff = d.catheterOff;
    m_pIbpModuleData->m_pStatus->zeroState = d.zeroStatus;
    m_pIbpModuleData->m_pStatus->calibrationState = d.calibrationStatus;


    m_modulePacket.ParamPackets++;
    m_modulePacket.WavePackets += d.waves.size();
    if(m_modulePacket.Count++ >= 1)
    {
        m_modulePacket.Count = 0;
        m_pIbpModuleData->m_ModuleInfo.packets.ParamPackets = m_modulePacket.ParamPackets;
        m_pIbpModuleData->m_ModuleInfo.packets.WavePackets = m_modulePacket.WavePackets;
        m_modulePacket.ParamPackets = 0;
        m_modulePacket.WavePackets = 0;
    }
    IbpWaveUnit *pw = new IbpWaveUnit[d.waves.size()];

    for(int i = 0; i < d.waves.size(); i++)
    {
        auto FrameData = d.waves.at(i);
//        pw[i].bVaild = !d.sensorOff;
        pw[i].bVaild = true;
        pw[i].wave = FrameData.ibpWave;
    }
    if(d.waves.size() <= m_pIbpModuleData->m_pWave->capacity())
    {
        m_pIbpModuleData->m_pWave->write(pw, d.waves.size());
    }
    else
    {
        qDebug() << "Ibp too much Data!!";
    }
    delete [] pw;

//    qDebug() << "module ibp set Data";

}

void ModuleIbp::getData(QVariant &value)
{
    value.setValue(m_pIbpModuleData);
}

