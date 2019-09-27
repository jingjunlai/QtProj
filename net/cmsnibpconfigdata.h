#ifndef CMSNIBPCONFIGDATA_H
#define CMSNIBPCONFIGDATA_H

#include "cmsconfigdata.h"

typedef enum
{
    CMS_NibpModeAdult,  // 成人模式
    CMS_NibpModeNeo,    // 婴儿模式
    CMS_NibpModePed,    // 儿童模式
    CMS_NibpModeHyper,  // 高血压模式
} CmsNibpMeasureMode;

struct NibpConfigField : public BaseConfigField
{
    enum
    {
        OP_ProbeInfo = 0x01,
    };

    CmsNibpMeasureMode measureMode = CMS_NibpModeAdult;
    CmsAlarmConfig<>   sysAlarmSetting;
    CmsAlarmConfig<>   diaAlarmSetting;
    CmsAlarmConfig<>   meanAlarmSetting;
    CmsAlarmConfig<>   sdpAlarmSetting;
};

class CmsNibpConfigData : public CmsConfigData
{
public:
    CmsNibpConfigData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsNibpConfigData();

    void setConfig(NibpConfigField config);

    NibpConfigField config() const;

protected:
    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

protected:
    NibpConfigField configField;
    static const int    baseSectionSize = 30;
};

#endif // CMSNIBPCONFIGDATA_H
