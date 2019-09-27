#ifndef CMSTEMPCONFIGDATA_H
#define CMSTEMPCONFIGDATA_H

#include "cmsconfigdata.h"

struct TempConfigField : public BaseConfigField
{
    enum
    {
        OP_ProbeInfo = 0x01,
    };

    bool isDualTemp = false;

    struct
    {
        uint8_t     subid;
        CmsAlarmConfig<>   alarmSetting;
    } temp[2];

    CmsAlarmConfig<>   deltaTAlarmSetting;
};

class CmsTempConfigData : public CmsConfigData
{
public:
    CmsTempConfigData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsTempConfigData();

    void setConfig(TempConfigField config);

    TempConfigField config() const;

protected:
    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

protected:
    TempConfigField configField;
    static const int    baseSectionSize = 35;
};

#endif // CMSTEMPCONFIGDATA_H
