#include "moduleco.h"
#include "mathtools.h"
#include "net/cmscomoduledata.h"
#include <QVariant>
#include <QDebug>
#include <QThread>

COParam::COParam(ModuleID module, LabelID label)
    : ModuleParam(module, label)
{

}

COStatus::COStatus(ModuleID module, LabelID label)
    : ModuleStatus(module, label)
{

}

CoModuleData::CoModuleData(ModuleID module, LabelID label)
    : ModuleData(module, label)
{
    m_pParam = new COParam(module, label);
    m_pStatus = new COStatus(module, label);
    reset();
}

CoModuleData::~CoModuleData()
{
    delete m_pParam;
    delete m_pStatus;
}

void CoModuleData::reset()
{
    m_pParam->reset();
    m_pStatus->reset();
}


ModuleCo::ModuleCo(ModuleProperty property, QObject *parent)
    : IModule(property, parent)
{
    m_pCoModuleData = new CoModuleData(property.moduleId, property.labelId);
}

ModuleCo::~ModuleCo()
{
    delete m_pCoModuleData;
}

void ModuleCo::unpackModuleData(const QByteArray &packet)
{
    Q_UNUSED(packet);
}

bool ModuleCo::setConfig(int id, const QVariant &value)
{
    Q_UNUSED(id);
    Q_UNUSED(value);
    return false;
}

bool ModuleCo::getConfig(int id, QVariant &value)
{
    Q_UNUSED(id);
    Q_UNUSED(value);
    return false;
}

void ModuleCo::setData(const QVariant &value)
{
    auto d = value.value<CoDataField>();
//    qDebug() << "module co set Data";

    Q_UNUSED(d);
}

void ModuleCo::getData(QVariant &value)
{
    value.setValue(m_pCoModuleData);
}

