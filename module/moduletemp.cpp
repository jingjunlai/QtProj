#include "moduletemp.h"
#include "mathtools.h"
#include "net/cmsTempmoduledata.h"
#include <QVariant>
#include <QDebug>
#include <QThread>

TempParam::TempParam(ModuleID module, LabelID label)
    : ModuleParam(module, label)
{

}

TempParam::~TempParam()
{

}

void TempParam::reset()
{
    data[0].temp = INVALID_DATA;
    data[1].temp = INVALID_DATA;
}

QList<ParamPoint> TempParam::toList()
{
    QList<ParamPoint> ret;

    ParamPoint point;
    point.label = labelId();
    point.subid = data[0].subid | 0x00;
    point.data = data[0].temp;
    ret.append(point);

    // 双体温才输出另外一个通道的数据
    if (dualTemp)
    {
        point.label = labelId();
        point.subid = data[1].subid | 0x80;
        point.data = data[1].temp;
        ret.append(point);
    }

    return ret;
}

TempStatus::TempStatus(ModuleID module, LabelID label)
    : ModuleStatus(module, label)
{

}

TempStatus::~TempStatus()
{

}

void TempStatus::reset()
{
    moduleState = Module_StateOnline;
    powerFrequency = 0;
    patientType = 0;
    errorCode = 0;
    probeState = Probe_StateNormal;
    measureStatus[0] = MeasureStatus::MeasureValid;
    measureStatus[1] = MeasureStatus::MeasureValid;
}

TempModuleData::TempModuleData(ModuleID module, LabelID label)
    : ModuleData(module, label)
{
    m_pParam = new TempParam(module, label);
    m_pStatus = new TempStatus(module, label);
    reset();
}

TempModuleData::~TempModuleData()
{
    delete m_pParam;
    delete m_pStatus;
}

void TempModuleData::reset()
{
    m_pParam->reset();
    m_pStatus->reset();
}

ModuleTemp::ModuleTemp(ModuleProperty property, QObject *parent)
    : IModule(property, parent)
{
    m_pTempModuleData = new TempModuleData(property.moduleId, property.labelId);
}

ModuleTemp::~ModuleTemp()
{
    delete m_pTempModuleData;
}

void ModuleTemp::unpackModuleData(const QByteArray &packet)
{
    Q_UNUSED(packet);
}

bool ModuleTemp::setConfig(int id, const QVariant &value)
{
    Q_UNUSED(id);
    Q_UNUSED(value);
    return false;
}

bool ModuleTemp::getConfig(int id, QVariant &value)
{
    Q_UNUSED(id);
    Q_UNUSED(value);
    return false;
}

void ModuleTemp::setData(const QVariant &value)
{
    auto d = value.value<TempDataField>();

    m_pTempModuleData->m_pParam->data[0].subid = d.temp[0].subid;
    m_pTempModuleData->m_pParam->data[0].temp = d.temp[0].data;
    m_pTempModuleData->m_pParam->data[1].subid = d.temp[1].subid;
    m_pTempModuleData->m_pParam->data[1].temp = d.temp[1].data;

    m_pTempModuleData->m_pStatus->measureStatus[0] = (TempStatus::MeasureStatus)d.temp[0].measureStatus;
    m_pTempModuleData->m_pStatus->measureStatus[1] = (TempStatus::MeasureStatus)d.temp[1].measureStatus;

    m_modulePacket.ParamPackets++;
    if(m_modulePacket.Count++ >= 1)
    {
        m_modulePacket.Count = 0;
        m_pTempModuleData->m_ModuleInfo.packets.ParamPackets = m_modulePacket.ParamPackets;
        m_modulePacket.ParamPackets = 0;
    }

//    qDebug() << "module Temp set Data = " << value;
}

void ModuleTemp::getData(QVariant &value)
{
    value.setValue(m_pTempModuleData);
}
