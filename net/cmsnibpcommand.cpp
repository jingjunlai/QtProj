#include "cmsnibpcommand.h"


CmsNibpCommand::CmsNibpCommand(CmsModuleID identifier, CmsLabelID label)
    : CmsCommandData(identifier, label)
{

}

CmsNibpCommand::~CmsNibpCommand()
{

}

void CmsNibpCommand::setCommand(const NibpCommandField &command)
{
    this->m_commandField = command;
}

bool CmsNibpCommand::decodePacket(const QByteArray &arr)
{
    Q_UNUSED(arr);
    return true;
}

QByteArray CmsNibpCommand::encodePacket() const
{
    uchar buf[3] = {0};

    buf[0] = m_commandField.command;
    buf[1] = m_commandField.param1;
    buf[2] = m_commandField.param2;
    QByteArray content;
    content = QByteArray((char*)buf, 3);

    return content;
}



