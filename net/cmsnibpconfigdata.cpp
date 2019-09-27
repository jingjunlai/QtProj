#include "cmsnibpconfigdata.h"
#include <QDebug>

CmsNibpConfigData::CmsNibpConfigData(CmsModuleID identifier, CmsLabelID label)
    : CmsConfigData(identifier, label)
{

}

CmsNibpConfigData::~CmsNibpConfigData()
{

}

void CmsNibpConfigData::setConfig(NibpConfigField config)
{
    this->configField = config;
}

NibpConfigField CmsNibpConfigData::config() const
{
    return this->configField;
}

bool CmsNibpConfigData::decodePacket(const QByteArray &arr)
{
    if (arr.size() < baseSectionSize)
        return false;

    auto buf = (const unsigned char *)arr.constData();
    int usedBytes = 0;

    configField.measureMode = (CmsNibpMeasureMode)(buf[4] & 0x03);

    configField.sysAlarmSetting.sw = GET_BIT(buf[5], 0) == 1;
    configField.sysAlarmSetting.level = (CmsAlarmLevel)((buf[5] >> 1) & 0x03);
    configField.sysAlarmSetting.limit[CmsAlarmConfig<>::up_index] = (buf[6] << 0) + (buf[7] << 8);
    configField.sysAlarmSetting.limit[CmsAlarmConfig<>::down_index] = (buf[8] << 0) + (buf[9] << 8);

    configField.diaAlarmSetting.sw = GET_BIT(buf[10], 0) == 1;
    configField.diaAlarmSetting.level = (CmsAlarmLevel)((buf[10] >> 1) & 0x03);
    configField.diaAlarmSetting.limit[CmsAlarmConfig<>::up_index] = (buf[11] << 0) + (buf[12] << 8);
    configField.diaAlarmSetting.limit[CmsAlarmConfig<>::down_index] = (buf[13] << 0) + (buf[14] << 8);

    configField.meanAlarmSetting.sw = GET_BIT(buf[15], 0) == 1;
    configField.meanAlarmSetting.level = (CmsAlarmLevel)((buf[15] >> 1) & 0x03);
    configField.meanAlarmSetting.limit[CmsAlarmConfig<>::up_index] = (buf[16] << 0) + (buf[17] << 8);
    configField.meanAlarmSetting.limit[CmsAlarmConfig<>::down_index] = (buf[18] << 0) + (buf[19] << 8);

    configField.sdpAlarmSetting.sw = GET_BIT(buf[20], 0) == 1;
    configField.sdpAlarmSetting.level = (CmsAlarmLevel)((buf[20] >> 1) & 0x03);
    configField.sdpAlarmSetting.limit[CmsAlarmConfig<>::up_index] = (buf[21] << 0) + (buf[22] << 8);
    configField.sdpAlarmSetting.limit[CmsAlarmConfig<>::down_index] = (buf[23] << 0) + (buf[24] << 8);

    configField.setOption(buf[29]);

    usedBytes += baseSectionSize;
    if ((configField.option() & NibpConfigField::OP_ProbeInfo) != 0)
    {
        if (usedBytes + CmsConfigData::probeSectionSize > arr.size())
        {
            qWarning() << QString("decode nibp probe info config error. need packet raw bytes: %1, rest bytes: %2")
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

QByteArray CmsNibpConfigData::encodePacket() const
{
    char buf[baseSectionSize] = {0};

    buf[4] = configField.measureMode & 0x03;


    configField.sysAlarmSetting.sw ? SET_BIT(buf[5], 0) : CLR_BIT(buf[5], 0);
    quint8 level = configField.sysAlarmSetting.level;
    buf[5] |= (level << 1);
    buf[6] = (configField.sysAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 0);
    buf[7] = (configField.sysAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 8);
    buf[8] = (configField.sysAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 0);
    buf[9] = (configField.sysAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 8);

    configField.diaAlarmSetting.sw ? SET_BIT(buf[10], 0) : CLR_BIT(buf[10], 0);
    level = configField.diaAlarmSetting.level;
    buf[10] |= (level << 1);
    buf[11] = (configField.diaAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 0);
    buf[12] = (configField.diaAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 8);
    buf[13] = (configField.diaAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 0);
    buf[14] = (configField.diaAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 8);

    configField.meanAlarmSetting.sw ? SET_BIT(buf[15], 0) : CLR_BIT(buf[15], 0);
    level = configField.meanAlarmSetting.level;
    buf[15] |= (level << 1);
    buf[16] = (configField.meanAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 0);
    buf[17] = (configField.meanAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 8);
    buf[18] = (configField.meanAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 0);
    buf[19] = (configField.meanAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 8);

    configField.sdpAlarmSetting.sw ? SET_BIT(buf[20], 0) : CLR_BIT(buf[20], 0);
    level = configField.sdpAlarmSetting.level;
    buf[20] |= (level << 1);
    buf[21] = (configField.sdpAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 0);
    buf[22] = (configField.sdpAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 8);
    buf[23] = (configField.sdpAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 0);
    buf[24] = (configField.sdpAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 8);

    buf[29] = configField.option();

    QByteArray content;
    content = QByteArray(buf + 4, sizeof(buf) - 4);
    if ((configField.option() & NibpConfigField::OP_ProbeInfo) != 0)
        content.append(encodeProbeInfo(configField.probeInfo));

    return content;
}


