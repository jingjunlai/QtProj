#include "imodule.h"

IModule::IModule(ModuleProperty property, QObject *parent) : QObject(parent)
    , m_ModuleProperty(property)
{

}

IModule::~IModule()
{

}

ModuleProperty IModule::moduleProperty() const
{
    return m_ModuleProperty;
}

bool IModule::Online() const
{
    return m_bOnline;
}

void IModule::setEnable()
{
    m_ModuleProperty.enable = true;
}

void IModule::setOnline()
{
    m_bOnline = true;
}
