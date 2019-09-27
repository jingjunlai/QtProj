#ifndef CMSSPO2CONFIGDATA_H
#define CMSSPO2CONFIGDATA_H

#include "cmsconfigdata.h"

struct Spo2ConfigField : public BaseConfigField
{
    enum
    {
        OP_ProbeInfo = 0x01,
        OP_Rainbow = 0x02,
    };

    enum
    {
        RB_SpStart,
        RB_SpCo = RB_SpStart,
        RB_SpMet,
        RB_SpHb,
        RB_SpOc,
        RB_Di,
        RB_deltaPi,
        RB_Pvi,
        RB_Max,
    };

    bool isDigitSpo2 = false;
    bool isSupportResp = false;
    bool isSupportPi = false;

    struct
    {
        bool sw;
        CmsAlarmLevel level;
        CmsAlarmLevel destLevel;
        quint8 highLimt;
        quint8 lowLimit;
    } spo2AlarmSetting;

    CmsAlarmConfig<>   piAlarmSetting;
    CmsAlarmConfig<>   respAlarmSetting;
    CmsAlarmConfig<>   rainbowAlarmSetting[RB_Max];
};

class CmsSpo2ConfigData : public CmsConfigData
{
public:
    CmsSpo2ConfigData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsSpo2ConfigData();

    void setConfig(Spo2ConfigField config);

    Spo2ConfigField config() const;

protected:
    virtual bool decodePacket(const QByteArray &arr);

    void decodeRainbowSection(const QByteArray &rawData);

    virtual QByteArray encodePacket() const;

    QByteArray encodeRainbowSection() const;

protected:
    Spo2ConfigField         configField;
    static const int        baseSectionSize = 23;
    static const int        rainbowSectionSize = 30;
};

#endif // CMSSPO2CONFIGDATA_H
