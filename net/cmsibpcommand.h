#ifndef CMSIBPCOMMAND_H
#define CMSIBPCOMMAND_H

#include "cmscommanddata.h"

struct IbpCommandField
{
    quint8 command = 0;
    quint8 param1 = 0;
    quint8 param2 = 0;
};

class CmsIbpCommand : public CmsCommandData
{
public:
    CmsIbpCommand(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsIbpCommand();

    void setCommand(const IbpCommandField &command);

protected:

    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

protected:

    IbpCommandField m_commandField;
};

#endif // CMSIBPCOMMAND_H
