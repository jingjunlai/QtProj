#include "modulenibp.h"
#include "mathtools.h"
#include "net/cmsnibpmoduledata.h"
#include <QVariant>
#include <QDebug>
#include <QThread>

NibpParam::NibpParam(ModuleID module, LabelID label)
    : ModuleParam(module, label)
{

}

NibpParam::~NibpParam()
{

}

void NibpParam::reset()
{
    sys = INVALID_DATA;
    dia = INVALID_DATA;
    map = INVALID_DATA;
    pr = INVALID_DATA;
//    sdp = INVALID_DATA;
}

QList<ParamPoint> NibpParam::toList()
{
    QList<ParamPoint> ret;

    ParamPoint point;
    point.label = labelId();
    point.subid = Sub_NibpSys;
    point.data = sys;
    ret.append(point);

    point.label = labelId();
    point.subid = Sub_NibpDia;
    point.data = dia;
    ret.append(point);

    point.label = labelId();
    point.subid = Sub_NibpMap;
    point.data = map;
    ret.append(point);

    point.label = labelId();
    point.subid = Sub_NibpPr;
    point.data = pr;
    ret.append(point);

    return ret;
}

NibpStatus::NibpStatus(ModuleID module, LabelID label)
    : ModuleStatus(module, label)
{

}


NibpStatus::~NibpStatus()
{

}

void NibpStatus::reset()
{
    patientType = 0;
    labelName = 0;
    errorCode = 0;
    physicalID = 0;
    moduleState = Module_StateOnline;
    labelName = LABEL_NIBP;
    normalValue = 0;
    workMode = 0;
    isWorking = 0;
    measureInterTime = 0;
    cuffPressure = 0;
    protectPressure = 0;
}

NibpWave::NibpWave(int size, ModuleID module, LabelID label)
    : ModuleWave(size, module, label)
{

}

NibpWave::~NibpWave()
{
}

void NibpWave::reset()
{

}

NibpModuleData::NibpModuleData(ModuleID module, LabelID label)
    : ModuleData(module, label)
{
    m_pParam = new NibpParam(module, label);
    m_pStatus = new NibpStatus(module, label);
    m_pWave = new RingBuffer<NibpWaveUnit>(100 * 3);
    reset();
}

NibpModuleData::~NibpModuleData()
{
    delete m_pParam;
    delete m_pStatus;
    delete m_pWave;
}

void NibpModuleData::reset()
{
    m_pParam->reset();
    m_pStatus->reset();
}

ModuleNibp::ModuleNibp(ModuleProperty property, QObject *parent)
    : IModule(property, parent)
{
    m_pNibpModuleData = new NibpModuleData(property.moduleId, property.labelId);
}

ModuleNibp::~ModuleNibp()
{
    delete m_pNibpModuleData;
}

void ModuleNibp::unpackModuleData(const QByteArray &packet)
{
    Q_UNUSED(packet);
}

bool ModuleNibp::setConfig(int id, const QVariant &value)
{
    Q_UNUSED(id);
    Q_UNUSED(value);
    return false;
}

bool ModuleNibp::getConfig(int id, QVariant &value)
{
    Q_UNUSED(id);
    Q_UNUSED(value);
    return false;
}

void ModuleNibp::setData(const QVariant &value)
{
    auto d = value.value<NibpDataField>();
    if((m_pNibpModuleData->m_pStatus->errorCode = (Gps::NIBP_ErrCode)d.errorCode) == Gps::Nibp_NoErr)
    {
        m_pNibpModuleData->m_pParam->sys = d.sys / 100;
        m_pNibpModuleData->m_pParam->dia = d.dia / 100;
        m_pNibpModuleData->m_pParam->map = d.map / 100;
        m_pNibpModuleData->m_pParam->pr = d.pr;
    }
    else
    {
        m_pNibpModuleData->m_pParam->sys = INVALID_DATA;
        m_pNibpModuleData->m_pParam->dia = INVALID_DATA;
        m_pNibpModuleData->m_pParam->map = INVALID_DATA;
        m_pNibpModuleData->m_pParam->pr = INVALID_DATA;
    }

    m_pNibpModuleData->m_pStatus->workMode = d.workMode;
    m_pNibpModuleData->m_pStatus->isWorking = d.working;

    m_modulePacket.ParamPackets++;
    m_modulePacket.WavePackets += d.waves.size();
    if(m_modulePacket.Count++ >= 1)
    {
        m_modulePacket.Count = 0;
        m_pNibpModuleData->m_ModuleInfo.packets.ParamPackets = m_modulePacket.ParamPackets;
        m_pNibpModuleData->m_ModuleInfo.packets.WavePackets = m_modulePacket.WavePackets;
        m_modulePacket.ParamPackets = 0;
        m_modulePacket.WavePackets = 0;
    }

    NibpWaveUnit *pw = new NibpWaveUnit[d.waves.size()];

    for(int i = 0; i < d.waves.size(); i++)
    {
        auto FrameData = d.waves.at(i);
        pw[i].bVaild = d.working;
        pw[i].wave = FrameData.wave;
    }
    if(d.waves.size() <= m_pNibpModuleData->m_pWave->capacity())
    {
        m_pNibpModuleData->m_pWave->write(pw, d.waves.size());
    }
    else
    {
        qDebug() << "Nibp too much Data!!";
    }
    delete [] pw;

//    qDebug() << "module nibp set Data";

}

void ModuleNibp::getData(QVariant &value)
{
    value.setValue(m_pNibpModuleData);
}
