#ifndef CMSCO2CONFIGDATA_H
#define CMSCO2CONFIGDATA_H


#include "cmsconfigdata.h"

struct Co2ConfigField : public BaseConfigField
{
    enum
    {
        OP_ProbeInfo = 0x01,
    };

    CmsAlarmConfig<>   etCo2AlarmSetting;
    CmsAlarmConfig<>   fiCo2AlarmSetting;
    CmsAlarmConfig<>   awrrAlarmSetting;
    bool                    apneaSwitch = false;
    quint8                  apneaTime = 0;

    // 协议为什么跟resp的不一样,这里需要两个变量来标识
};

class CmsCo2ConfigData : public CmsConfigData
{
public:
    CmsCo2ConfigData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsCo2ConfigData();

    void setConfig(Co2ConfigField config);

    Co2ConfigField config() const;

protected:
    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

protected:
    Co2ConfigField configField;
    static const int    baseSectionSize = 25;
};

#endif // CMSCO2CONFIGDATA_H
