#include "cmssyscommand.h"

CmsSysCommand::CmsSysCommand(CmsModuleID identifier, CmsLabelID label)
    : CmsCommandData(identifier, label)
{

}

CmsSysCommand::~CmsSysCommand()
{

}

void CmsSysCommand::setCommand(const SysCommandField &command)
{
    this->m_commandField = command;
}

SysCommandField CmsSysCommand::command() const
{
    return this->m_commandField;
}

bool CmsSysCommand::decodePacket(const QByteArray &arr)
{
    if(arr.at(10) <= maxType)
    {
        m_commandField.user.type = (CmsUserType)arr.at(10);
    }
    return true;
}

QByteArray CmsSysCommand::encodePacket() const
{
    uchar buf[3] = {0};

    QByteArray content;
    content = QByteArray((char*)buf, 3);

    return content;
}
