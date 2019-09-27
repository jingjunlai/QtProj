#ifndef CMSNIBPCOMMAND_H
#define CMSNIBPCOMMAND_H

#include "cmscommanddata.h"

struct NibpCommandField
{
    quint8 command = 0;
    quint8 param1 = 0;
    quint8 param2 = 0;
};

class CmsNibpCommand : public CmsCommandData
{
public:
    CmsNibpCommand(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsNibpCommand();

    void setCommand(const NibpCommandField &command);

protected:

    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

protected:

    NibpCommandField m_commandField;
};

#endif // CMSNIBPCOMMAND_H
