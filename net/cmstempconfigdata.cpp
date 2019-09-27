#include "cmstempconfigdata.h"
#include <QDebug>

CmsTempConfigData::CmsTempConfigData(CmsModuleID identifier, CmsLabelID label)
    : CmsConfigData(identifier, label)
{

}

CmsTempConfigData::~CmsTempConfigData()
{

}

void CmsTempConfigData::setConfig(TempConfigField config)
{
    this->configField = config;
}

TempConfigField CmsTempConfigData::config() const
{
    return this->configField;
}

bool CmsTempConfigData::decodePacket(const QByteArray &arr)
{
    if (arr.size() < baseSectionSize)
        return false;

    auto buf = (const unsigned char *)arr.constData();
    int usedBytes = 0;

    configField.isDualTemp = GET_BIT(buf[4], 0) == 1;

    // temp1
    configField.temp[0].subid = buf[5];
    configField.temp[0].alarmSetting.sw = GET_BIT(buf[6], 0) == 1;
    configField.temp[0].alarmSetting.level = (CmsAlarmLevel)(buf[6] >> 1);
    configField.temp[0].alarmSetting.limit[CmsAlarmConfig<>::up_index] = (buf[7] << 0) + (buf[8] << 8);
    configField.temp[0].alarmSetting.limit[CmsAlarmConfig<>::down_index] = (buf[9] << 0) + (buf[10] << 8);

    // temp2
    configField.temp[1].subid = buf[15];
    configField.temp[1].alarmSetting.sw = GET_BIT(buf[16], 0) == 1;
    configField.temp[1].alarmSetting.level = (CmsAlarmLevel)(buf[16] >> 1);
    configField.temp[1].alarmSetting.limit[CmsAlarmConfig<>::up_index] = (buf[17] << 0) + (buf[18] << 8);
    configField.temp[1].alarmSetting.limit[CmsAlarmConfig<>::down_index] = (buf[19] << 0) + (buf[20] << 8);

    // deltaT
    configField.deltaTAlarmSetting.sw = GET_BIT(buf[25], 0) == 1;
    configField.deltaTAlarmSetting.level = (CmsAlarmLevel)(buf[25] >> 1);
    configField.deltaTAlarmSetting.limit[CmsAlarmConfig<>::up_index] = (buf[26] << 0) + (buf[27] << 8);
    configField.deltaTAlarmSetting.limit[CmsAlarmConfig<>::down_index] = (buf[28] << 0) + (buf[29] << 8);

    configField.setOption(buf[34]);

    usedBytes += baseSectionSize;
    if ((configField.option() & TempConfigField::OP_ProbeInfo) != 0)
    {
        if (usedBytes + CmsConfigData::probeSectionSize > arr.size())
        {
            qWarning() << QString("decode temp probe info config error. need packet raw bytes: %1, rest bytes: %2")
                       .arg(CmsConfigData::probeSectionSize)
                       .arg(arr.size() - usedBytes);
            return false;
        }

        QByteArray tmp((const char *)buf + usedBytes, CmsConfigData::probeSectionSize);
        decodeProbeInfo(tmp, &configField.probeInfo);
        usedBytes += CmsConfigData::probeSectionSize;
    }

    return true;
}

QByteArray CmsTempConfigData::encodePacket() const
{
    char buf[baseSectionSize] = {0};

    configField.isDualTemp ? SET_BIT(buf[4], 0) : CLR_BIT(buf[4], 0);

    // temp1
    buf[5] = configField.temp[0].subid;
    configField.temp[0].alarmSetting.sw ? SET_BIT(buf[6], 0) : CLR_BIT(buf[6], 0);
    quint8 level = configField.temp[0].alarmSetting.level;
    buf[6] |= (level << 1);
    buf[7] = (configField.temp[0].alarmSetting.limit[CmsAlarmConfig<>::up_index] >> 0);
    buf[8] = (configField.temp[0].alarmSetting.limit[CmsAlarmConfig<>::up_index] >> 8);
    buf[9] = (configField.temp[0].alarmSetting.limit[CmsAlarmConfig<>::down_index] >> 0);
    buf[10] = (configField.temp[0].alarmSetting.limit[CmsAlarmConfig<>::down_index] >> 8);

    // temp2
    buf[15] = configField.temp[1].subid;
    configField.temp[1].alarmSetting.sw ? SET_BIT(buf[16], 0) : CLR_BIT(buf[16], 0);
    level = configField.temp[1].alarmSetting.level;
    buf[16] |= (level << 1);
    buf[17] = (configField.temp[1].alarmSetting.limit[CmsAlarmConfig<>::up_index] >> 0);
    buf[18] = (configField.temp[1].alarmSetting.limit[CmsAlarmConfig<>::up_index] >> 8);
    buf[19] = (configField.temp[1].alarmSetting.limit[CmsAlarmConfig<>::down_index] >> 0);
    buf[20] = (configField.temp[1].alarmSetting.limit[CmsAlarmConfig<>::down_index] >> 8);

    // deltaT
    configField.deltaTAlarmSetting.sw ? SET_BIT(buf[25], 0) : CLR_BIT(buf[25], 0);
    level = configField.deltaTAlarmSetting.level;
    buf[25] |= (level << 1);
    buf[26] = (configField.deltaTAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 0);
    buf[27] = (configField.deltaTAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 8);
    buf[28] = (configField.deltaTAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 0);
    buf[29] = (configField.deltaTAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 8);

    buf[34] = configField.option();

    QByteArray content;
    content = QByteArray(buf + 4, sizeof(buf) - 4);
    if ((configField.option() & TempConfigField::OP_ProbeInfo) != 0)
        content.append(encodeProbeInfo(configField.probeInfo));

    return content;
}

