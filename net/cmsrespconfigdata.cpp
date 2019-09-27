#include "cmsrespconfigdata.h"
#include <QDebug>

CmsRespConfigData::CmsRespConfigData(CmsModuleID identifier, CmsLabelID label)
    : CmsConfigData(identifier, label)
{

}

CmsRespConfigData::~CmsRespConfigData()
{

}

void CmsRespConfigData::setConfig(RespConfigField config)
{
    this->configField = config;
}

RespConfigField CmsRespConfigData::config() const
{
    return this->configField;
}

bool CmsRespConfigData::decodePacket(const QByteArray &arr)
{
    if (arr.size() < baseSectionSize)
        return false;

    auto buf = (const unsigned char *)arr.constData();
    int usedBytes = 0;

    configField.apneaTime = buf[4] & 0x7f;

    configField.rrAlarmSetting.sw = GET_BIT(buf[5], 0) == 1;
    configField.rrAlarmSetting.level = (CmsAlarmLevel)((buf[5] >> 1) & 0x03);
    configField.rrAlarmSetting.limit[CmsAlarmConfig<>::up_index] = buf[6];
    configField.rrAlarmSetting.limit[CmsAlarmConfig<>::down_index] = buf[7];

    configField.setOption(buf[14]);
    usedBytes += baseSectionSize;
    if ((configField.option() & RespConfigField::OP_ProbeInfo) != 0)
    {
        if (usedBytes + CmsConfigData::probeSectionSize > arr.size())
        {
            qWarning() << QString("decode resp probe info config error. need packet raw bytes: %1, rest bytes: %2")
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

QByteArray CmsRespConfigData::encodePacket() const
{
    char buf[baseSectionSize] = {0};

    buf[4] = configField.apneaTime;

    configField.rrAlarmSetting.sw ? SET_BIT(buf[5], 0) : CLR_BIT(buf[5], 0);
    quint8 level = configField.rrAlarmSetting.level;
    buf[5] |= (level << 1);
    buf[6] = configField.rrAlarmSetting.limit[CmsAlarmConfig<>::up_index];
    buf[7] = configField.rrAlarmSetting.limit[CmsAlarmConfig<>::down_index];

    buf[14] = configField.option();

    QByteArray content;
    content = QByteArray(buf + 4, sizeof(buf) - 4);
    if ((configField.option() & RespConfigField::OP_ProbeInfo) != 0)
        content.append(encodeProbeInfo(configField.probeInfo));

    return content;
}

