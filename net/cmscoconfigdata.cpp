#include "cmscoconfigdata.h"
#include <QDebug>

CmsCoConfigData::CmsCoConfigData(CmsModuleID identifier, CmsLabelID label)
    : CmsConfigData(identifier, label)
{

}

CmsCoConfigData::~CmsCoConfigData()
{

}

void CmsCoConfigData::setConfig(CoConfigField config)
{
    this->configField = config;
}

CoConfigField CmsCoConfigData::config() const
{
    return this->configField;
}

bool CmsCoConfigData::decodePacket(const QByteArray &arr)
{
    if (arr.size() < baseSectionSize)
        return false;

//    auto buf = (const unsigned char *)arr.constData();
//    int usedBytes = 0;

//    configField.etCo2AlarmSetting.sw = GET_BIT(buf[4], 0) == 1;
//    configField.fiCo2AlarmSetting.sw = GET_BIT(buf[4], 1) == 1;
//    configField.awrrAlarmSetting.sw = GET_BIT(buf[4], 2) == 1;
//    configField.apneaSwitch = GET_BIT(buf[4], 3) == 1;

//    configField.etCo2AlarmSetting.level = (CmsAlarmLevel)(buf[5] & 0x03);
//    configField.fiCo2AlarmSetting.level = (CmsAlarmLevel)(buf[6] & 0x03);
//    configField.awrrAlarmSetting.level = (CmsAlarmLevel)(buf[7] & 0x03);

//    configField.etCo2AlarmSetting.limit[CmsAlarmConfig<>::up_index] = (buf[8] << 0) + (buf[9] << 8);
//    configField.etCo2AlarmSetting.limit[CmsAlarmConfig<>::down_index] = (buf[10] << 0) + (buf[11] << 8);

//    configField.fiCo2AlarmSetting.limit[CmsAlarmConfig<>::up_index] = (buf[12] << 0) + (buf[13] << 8);
//    configField.fiCo2AlarmSetting.limit[CmsAlarmConfig<>::down_index] = (buf[14] << 0) + (buf[15] << 8);

//    configField.awrrAlarmSetting.limit[CmsAlarmConfig<>::up_index] = (buf[16] << 0) + (buf[17] << 8);
//    configField.awrrAlarmSetting.limit[CmsAlarmConfig<>::down_index] = (buf[18] << 0) + (buf[19] << 8);

//    configField.apneaTime = buf[20];

//    usedBytes += baseSectionSize;
    return true;
}

QByteArray CmsCoConfigData::encodePacket() const
{
    char buf[baseSectionSize] = {0};

//    configField.etCo2AlarmSetting.sw ? SET_BIT(buf[4], 0) : CLR_BIT(buf[4], 0);
//    configField.fiCo2AlarmSetting.sw ? SET_BIT(buf[4], 1) : CLR_BIT(buf[4], 1);
//    configField.awrrAlarmSetting.sw ? SET_BIT(buf[4], 2) : CLR_BIT(buf[4], 2);
//    configField.apneaSwitch ? SET_BIT(buf[4], 3) : CLR_BIT(buf[4], 3);

//    quint8 level = configField.etCo2AlarmSetting.level;
//    buf[5] = (level & 0x03);
//    level = configField.fiCo2AlarmSetting.level;
//    buf[6] = (level & 0x03);
//    level = configField.awrrAlarmSetting.level;
//    buf[7] = (level & 0x03);

//    buf[8] = (configField.etCo2AlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 0);
//    buf[9] = (configField.etCo2AlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 8);
//    buf[10] = (configField.etCo2AlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 0);
//    buf[11] = (configField.etCo2AlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 8);

//    buf[12] = (configField.fiCo2AlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 0);
//    buf[13] = (configField.fiCo2AlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 8);
//    buf[14] = (configField.fiCo2AlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 0);
//    buf[15] = (configField.fiCo2AlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 8);

//    buf[16] = (configField.awrrAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 0);
//    buf[17] = (configField.awrrAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 8);
//    buf[18] = (configField.awrrAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 0);
//    buf[19] = (configField.awrrAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 8);

//    buf[20] = configField.apneaTime;

    QByteArray content;
    content = QByteArray(buf + 4, sizeof(buf) - 4);
    return content;
}

