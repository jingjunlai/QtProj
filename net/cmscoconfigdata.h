#ifndef CMSCOCONFIGDATA_H
#define CMSCOCONFIGDATA_H

#include "cmsconfigdata.h"

struct CoConfigField : public BaseConfigField
{
    enum
    {
        OP_ProbeInfo = 0x01,
    };
};

class CmsCoConfigData : public CmsConfigData
{
public:
    CmsCoConfigData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsCoConfigData();

    void setConfig(CoConfigField config);

    CoConfigField config() const;

protected:
    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

protected:
    CoConfigField configField;
    static const int    baseSectionSize = 22;
};

#endif // CMSCOCONFIGDATA_H
