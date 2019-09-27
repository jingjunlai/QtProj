#ifndef CMSIBPCONFIGDATA_H
#define CMSIBPCONFIGDATA_H

#include "cmsconfigdata.h"

struct IbpConfigField : public BaseConfigField
{
    enum
    {
        OP_ProbeInfo = 0x01,
    };

    CmsAlarmConfig<qint16> sysAlarmSetting;
    CmsAlarmConfig<qint16> diaAlarmSetting;
    CmsAlarmConfig<qint16> meanAlarmSetting;
};

class CmsIbpConfigData : public CmsConfigData
{
public:
    CmsIbpConfigData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsIbpConfigData();

    void setConfig(IbpConfigField config);

    IbpConfigField config() const;

protected:
    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

protected:
    IbpConfigField configField;
    static const int    baseSectionSize = 22;
};

#endif // CMSIBPCONFIGDATA_H
