#include "cmsibpconfigdata.h"
#include <QDebug>

CmsIbpConfigData::CmsIbpConfigData(CmsModuleID identifier, CmsLabelID label)
    : CmsConfigData(identifier, label)
{

}

CmsIbpConfigData::~CmsIbpConfigData()
{

}

void CmsIbpConfigData::setConfig(IbpConfigField config)
{
    this->configField = config;
}

IbpConfigField CmsIbpConfigData::config() const
{
    return this->configField;
}

bool CmsIbpConfigData::decodePacket(const QByteArray &arr)
{
    if (arr.size() < baseSectionSize)
        return false;

    auto buf = (const unsigned char *)arr.constData();
    int usedBytes = 0;

    configField.sysAlarmSetting.sw = GET_BIT(buf[4], 0) == 1;
    configField.sysAlarmSetting.level = (CmsAlarmLevel)((buf[4] >> 1) & 0x03);
    configField.diaAlarmSetting.sw = GET_BIT(buf[5], 0) == 1;
    configField.diaAlarmSetting.level = (CmsAlarmLevel)((buf[5] >> 1) & 0x03);
    configField.meanAlarmSetting.sw = GET_BIT(buf[6], 0) == 1;
    configField.meanAlarmSetting.level = (CmsAlarmLevel)((buf[6] >> 1) & 0x03);

    configField.sysAlarmSetting.limit[CmsAlarmConfig<>::up_index] = (buf[7] << 0) + (buf[8] << 8) - 100 * 100;
    configField.sysAlarmSetting.limit[CmsAlarmConfig<>::down_index] = (buf[9] << 0) + (buf[10] << 8) - 100 * 100;

    configField.diaAlarmSetting.limit[CmsAlarmConfig<>::up_index] = (buf[11] << 0) + (buf[12] << 8) - 100 * 100;
    configField.diaAlarmSetting.limit[CmsAlarmConfig<>::down_index] = (buf[13] << 0) + (buf[14] << 8) - 100 * 100;

    configField.meanAlarmSetting.limit[CmsAlarmConfig<>::up_index] = (buf[15] << 0) + (buf[16] << 8) - 100 * 100;
    configField.meanAlarmSetting.limit[CmsAlarmConfig<>::down_index] = (buf[17] << 0) + (buf[18] << 8) - 100 * 100;

    configField.setOption(buf[21]);
    usedBytes += baseSectionSize;
    if ((configField.option() & IbpConfigField::OP_ProbeInfo) != 0)
    {
        if (usedBytes + CmsConfigData::probeSectionSize > arr.size())
        {
            qWarning() << QString("decode ibp probe info config error. need packet raw bytes: %1, rest bytes: %2")
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

QByteArray CmsIbpConfigData::encodePacket() const
{
    char buf[baseSectionSize] = {0};

    configField.sysAlarmSetting.sw ? SET_BIT(buf[4], 0) : CLR_BIT(buf[4], 0);
    quint8 level = configField.sysAlarmSetting.level;
    buf[4] |= (level << 1);

    configField.diaAlarmSetting.sw ? SET_BIT(buf[5], 0) : CLR_BIT(buf[5], 0);
    level = configField.diaAlarmSetting.level;
    buf[5] |= (level << 1);

    configField.meanAlarmSetting.sw ? SET_BIT(buf[6], 0) : CLR_BIT(buf[6], 0);
    level = configField.meanAlarmSetting.level;
    buf[6] |= (level << 1);

    quint16 sysHigh = configField.sysAlarmSetting.limit[CmsAlarmConfig<>::up_index] + 100 * 100;
    buf[7] = (sysHigh >> 0);
    buf[8] = (sysHigh >> 8);
    quint16 sysLow = configField.sysAlarmSetting.limit[CmsAlarmConfig<>::down_index] + 100 * 100;
    buf[9] = (sysLow >> 0);
    buf[10] = (sysLow >> 8);

    quint16 diaHigh = configField.diaAlarmSetting.limit[CmsAlarmConfig<>::up_index] + 100 * 100;
    buf[11] = (diaHigh >> 0);
    buf[12] = (diaHigh >> 8);
    quint16 diaLow = configField.diaAlarmSetting.limit[CmsAlarmConfig<>::down_index] + 100 * 100;
    buf[13] = (diaLow >> 0);
    buf[14] = (diaLow >> 8);

    quint16 meanHigh = configField.meanAlarmSetting.limit[CmsAlarmConfig<>::up_index] + 100 * 100;
    buf[15] = (meanHigh >> 0);
    buf[16] = (meanHigh >> 8);
    quint16 meanLow = configField.meanAlarmSetting.limit[CmsAlarmConfig<>::down_index] + 100 * 100;
    buf[17] = (meanLow >> 0);
    buf[18] = (meanLow >> 8);

    buf[21] = configField.option();

    QByteArray content;
    content = QByteArray(buf + 4, sizeof(buf) - 4);
    if ((configField.option() & IbpConfigField::OP_ProbeInfo) != 0)
        content.append(encodeProbeInfo(configField.probeInfo));

    return content;
}

