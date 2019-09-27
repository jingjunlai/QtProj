#ifndef CMSRESPCONFIGDATA_H
#define CMSRESPCONFIGDATA_H

#include "cmsconfigdata.h"
#include "cmsecgmoduledata.h"

struct RespConfigField : public BaseConfigField
{
    enum
    {
        OP_ProbeInfo = 0x01,
    };

    quint8                  apneaTime;       //窒息报警：0-关闭，5-120窒息报警时间
    CmsAlarmConfig<>   rrAlarmSetting;
};

class CmsRespConfigData : public CmsConfigData
{
public:
    CmsRespConfigData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsRespConfigData();

    void setConfig(RespConfigField config);

    RespConfigField config() const;

protected:
    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

protected:
    RespConfigField configField;
    static const int    baseSectionSize = 15;
};

#endif // CMSRESPCONFIGDATA_H
