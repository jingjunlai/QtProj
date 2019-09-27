#include "moduledata.h"

ModuleData::ModuleData(ModuleID module, LabelID label)
    : mModuleId(module)
    , mLabelId(label)
{

}

ModuleData::~ModuleData()
{

}

ModuleData::ModuleData(const ModuleData &other)
{
    mModuleId = other.mModuleId;
    mLabelId = other.mLabelId;
}

ModuleData &ModuleData::operator=(const ModuleData &other)
{
    mModuleId = other.mModuleId;
    mLabelId = other.mLabelId;
    return *this;
}

 ModuleID ModuleData::moduleId() const
{
    return mModuleId;
}

LabelID ModuleData::labelId() const
{
    return mLabelId;
}

ModuleParam::ModuleParam(ModuleID module, LabelID label)
    : ModuleParamBase(module, label)
{

}

ModuleParam::~ModuleParam()
{

}

ModuleStatus::ModuleStatus(ModuleID module, LabelID label)
    : ModuleData(module, label)
{

}

ModuleStatus::~ModuleStatus()
{

}

void ModuleData::reset()
{

}

ModuleParamBase::ModuleParamBase(ModuleID module, LabelID label)
    : ModuleData(module, label)
{

}

QList<ParamPoint> ModuleParamBase::toList()
{
    return QList<ParamPoint>();
}

ModuleWaveBase::ModuleWaveBase(ModuleID module, LabelID label)
    : ModuleData(module, label)
{

}

QList<WavePoint> ModuleWaveBase::toList(WaveID id)
{
    Q_UNUSED(id);
    return QList<WavePoint>();
}



