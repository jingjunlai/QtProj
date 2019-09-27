#ifndef CMSCONFIGDATA_H
#define CMSCONFIGDATA_H

#include "icmsdata.h"

template <typename T = quint16>
struct CmsAlarmConfig
{
    enum
    {
        up_index = 0,
        down_index = 1,
        max_index = 2
    };

    bool            sw = false;
    CmsAlarmLevel   level = CMS_AlarmMidLevel;
    T               limit[max_index];
};

struct BaseConfigField
{
    CmsProbeInfo probeInfo;         // 探头信息
    quint8       configOption = 0;      // 配置选项信息

    void setOption(quint8 option)
    {
        configOption = option;
    }

    quint8 option() const
    {
        return configOption;
    }
};

class CmsConfigData : public ICmsData
{
public:
    CmsConfigData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsConfigData();

    virtual bool fromByteArray(const QByteArray &arr);

    virtual QByteArray toByteArray() const;

protected:
    virtual bool decodePacket(const QByteArray &arr) = 0;

    virtual QByteArray encodePacket() const = 0;

    void decodeProbeInfo(QByteArray rawData, CmsProbeInfo *info) const;

    QByteArray encodeProbeInfo(CmsProbeInfo info) const;

protected:
    CmsModuleID         identifier = CMS_ModuleNull;
    CmsLabelID          label = CMS_LabelNull;
    static const int    probeSectionSize = 16;
};

#endif // CMSCONFIGDATA_H
