#include "cmsspo2configdata.h"
#include <QDebug>

CmsSpo2ConfigData::CmsSpo2ConfigData(CmsModuleID identifier, CmsLabelID label)
    : CmsConfigData(identifier, label)
{

}

CmsSpo2ConfigData::~CmsSpo2ConfigData()
{

}

void CmsSpo2ConfigData::setConfig(Spo2ConfigField config)
{
    this->configField = config;
}

Spo2ConfigField CmsSpo2ConfigData::config() const
{
    return this->configField;
}

bool CmsSpo2ConfigData::decodePacket(const QByteArray &arr)
{
    if (arr.size() < baseSectionSize)
        return false;

    auto buf = (const unsigned char *)arr.constData();
    int usedBytes = 0;

    configField.isDigitSpo2 = GET_BIT(buf[4], 5) == 1;
    configField.isSupportResp = GET_BIT(buf[4], 6) == 1;
    configField.isSupportPi = GET_BIT(buf[4], 7) == 1;

    configField.spo2AlarmSetting.sw = GET_BIT(buf[4], 0) == 1;
    configField.spo2AlarmSetting.level = (CmsAlarmLevel)((buf[4] >> 1) & 0x03);
    configField.spo2AlarmSetting.destLevel = (CmsAlarmLevel)((buf[4] >> 3) & 0x03);
    configField.spo2AlarmSetting.highLimt = buf[5];
    configField.spo2AlarmSetting.lowLimit = buf[6];

    configField.piAlarmSetting.sw = GET_BIT(buf[7], 0) == 1;
    configField.piAlarmSetting.level = (CmsAlarmLevel)(buf[7] >> 1);
    configField.piAlarmSetting.limit[CmsAlarmConfig<>::up_index] = (buf[8] << 0) + (buf[9] << 8);
    configField.piAlarmSetting.limit[CmsAlarmConfig<>::down_index] = (buf[10] << 0) + (buf[11] << 8);

    configField.respAlarmSetting.sw = GET_BIT(buf[12], 0) == 1;
    configField.respAlarmSetting.level = (CmsAlarmLevel)(buf[12] >> 1);
    configField.respAlarmSetting.limit[CmsAlarmConfig<>::up_index] = (buf[13] << 0) + (buf[14] << 8);
    configField.respAlarmSetting.limit[CmsAlarmConfig<>::down_index] = (buf[15] << 0) + (buf[16] << 8);

    configField.setOption(buf[22]);
    usedBytes += baseSectionSize;
    if ((configField.option() & Spo2ConfigField::OP_ProbeInfo) != 0)
    {
        if (usedBytes + CmsConfigData::probeSectionSize > arr.size())
        {
            qWarning() << QString("decode spo2 probe info config error. need packet raw bytes: %1, rest bytes: %2")
                       .arg(CmsConfigData::probeSectionSize)
                       .arg(arr.size() - usedBytes);
            return false;
        }

        QByteArray tmp((const char *)buf + usedBytes, CmsConfigData::probeSectionSize);
        decodeProbeInfo(tmp, &configField.probeInfo);
        usedBytes += CmsConfigData::probeSectionSize;
    }

    if ((configField.option() & Spo2ConfigField::OP_Rainbow) != 0)
    {
        if (usedBytes + rainbowSectionSize > arr.size())
        {
            qWarning() << QString("decode spo2 rainbow config error. need packet raw bytes: %1, rest bytes: %2")
                       .arg(rainbowSectionSize)
                       .arg(arr.size() - usedBytes);
            return false;
        }

        QByteArray tmp((const char *)buf + usedBytes, rainbowSectionSize);
        decodeRainbowSection(tmp);
        usedBytes += rainbowSectionSize;
    }

    return true;
}

void CmsSpo2ConfigData::decodeRainbowSection(const QByteArray &rawData)
{
    if (rawData.size() != rainbowSectionSize)
        return;

    auto buf = (const unsigned char *)rawData.constData();
    for (int i = Spo2ConfigField::RB_SpStart, index = 0; i < Spo2ConfigField::RB_Max; i++, index += 5)
    {
        configField.rainbowAlarmSetting[i].sw = GET_BIT(buf[index], 0) == 1;
        configField.rainbowAlarmSetting[i].level = (CmsAlarmLevel)(buf[index] >> 1);
        configField.rainbowAlarmSetting[i].limit[CmsAlarmConfig<>::up_index] = (buf[index + 1] << 0) + (buf[index + 2] << 8);
        configField.rainbowAlarmSetting[i].limit[CmsAlarmConfig<>::down_index] = (buf[index + 3] << 0) + (buf[index + 4] << 8);
    }
}

QByteArray CmsSpo2ConfigData::encodePacket() const
{
    char buf[baseSectionSize] = {0};

    configField.isDigitSpo2 ? SET_BIT(buf[4], 5) : CLR_BIT(buf[4], 5);
    configField.isSupportResp ? SET_BIT(buf[4], 6) : CLR_BIT(buf[4], 6);
    configField.isSupportPi ? SET_BIT(buf[4], 7) : CLR_BIT(buf[4], 7);

    configField.spo2AlarmSetting.sw ? SET_BIT(buf[4], 0) : CLR_BIT(buf[4], 0);
    quint8 level = configField.spo2AlarmSetting.level;
    buf[4] |= (level << 1);
    level = configField.spo2AlarmSetting.destLevel;
    buf[4] |= (level << 3);
    buf[5] = configField.spo2AlarmSetting.highLimt;
    buf[6] = configField.spo2AlarmSetting.lowLimit;

    configField.piAlarmSetting.sw ? SET_BIT(buf[7], 0) : CLR_BIT(buf[7], 0);
    level = configField.piAlarmSetting.level;
    buf[7] |= (level << 1);
    buf[8] = (configField.piAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 0);
    buf[9] = (configField.piAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 8);
    buf[10] = (configField.piAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 0);
    buf[11] = (configField.piAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 8);

    configField.respAlarmSetting.sw ? SET_BIT(buf[12], 0) : CLR_BIT(buf[12], 0);
    level = configField.respAlarmSetting.level;
    buf[12] |= (level << 1);
    buf[13] = (configField.respAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 0);
    buf[14] = (configField.respAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 8);
    buf[15] = (configField.respAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 0);
    buf[16] = (configField.respAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 8);

    buf[22] = configField.option();

    QByteArray content;
    content = QByteArray(buf + 4, sizeof(buf) - 4);
    if ((configField.option() & Spo2ConfigField::OP_ProbeInfo) != 0)
        content.append(encodeProbeInfo(configField.probeInfo));

    if ((configField.option() & Spo2ConfigField::OP_Rainbow) != 0)
        content.append(encodeRainbowSection());

    return content;
}

QByteArray CmsSpo2ConfigData::encodeRainbowSection() const
{
    char buf[35] = {0};
    for (int i = Spo2ConfigField::RB_SpStart, index = 0; i < Spo2ConfigField::RB_Max; i++, index += 5)
    {
        configField.rainbowAlarmSetting[i].sw ? SET_BIT(buf[index], 0) : CLR_BIT(buf[index], 0);
        quint8 level = configField.rainbowAlarmSetting[i].level;
        buf[index] |= (level << 1);
        buf[index + 1] = (configField.rainbowAlarmSetting[i].limit[CmsAlarmConfig<>::up_index] >> 0);
        buf[index + 2] = (configField.rainbowAlarmSetting[i].limit[CmsAlarmConfig<>::up_index] >> 8);
        buf[index + 3] = (configField.rainbowAlarmSetting[i].limit[CmsAlarmConfig<>::down_index] >> 0);
        buf[index + 4] = (configField.rainbowAlarmSetting[i].limit[CmsAlarmConfig<>::down_index] >> 8);
    }

    return QByteArray(buf, sizeof(buf));
}

