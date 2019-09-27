#include "cmsecgconfigdata.h"
#include <QDebug>

CmsEcgConfigData::CmsEcgConfigData(CmsModuleID identifier, CmsLabelID label)
    : CmsConfigData(identifier, label)
{
}

CmsEcgConfigData::~CmsEcgConfigData()
{

}

void CmsEcgConfigData::setConfig(EcgConfigField config)
{
    this->configField = config;
}

EcgConfigField CmsEcgConfigData::config() const
{
    return this->configField;
}

bool CmsEcgConfigData::decodePacket(const QByteArray &arr)
{
    if (arr.size() < baseSectionSize)
        return false;

    auto buf = (const unsigned char *)arr.constData();
    int usedBytes = 0;

    configField.workMode = (CmsEcgWorkMode)buf[4];
    configField.leadType = (CmsEcgLeadType)buf[5];
    configField.arrSwitch = GET_BIT(buf[6], 0) == 1;
    configField.stSwitch = GET_BIT(buf[6], 1) == 1;
    configField.paceSwitch = GET_BIT(buf[6], 2) == 1;
    configField.monitorLead = (CmsECGLeadIndex)buf[7];

    configField.hrAlarmSetting.sw = GET_BIT(buf[8], 0);
    configField.hrAlarmSetting.level = (CmsAlarmLevel)(buf[8] >> 1);
    configField.hrAlarmSetting.limit[CmsAlarmConfig<>::up_index] = (buf[9] << 0) + (buf[10] << 8);
    configField.hrAlarmSetting.limit[CmsAlarmConfig<>::down_index] = (buf[11] << 0) + (buf[12] << 8);

    configField.setOption(buf[15]);
    usedBytes += baseSectionSize;
    if ((configField.option() & EcgConfigField::OP_ProbeInfo) != 0)
    {
        if (usedBytes + CmsConfigData::probeSectionSize > arr.size())
            return false;

        QByteArray tmp((const char *)buf + usedBytes, CmsConfigData::probeSectionSize);
        decodeProbeInfo(tmp, &configField.probeInfo);
        usedBytes += CmsConfigData::probeSectionSize;
    }

    if ((configField.option() & EcgConfigField::OP_ArrConfig) != 0)
    {
        if (usedBytes + arrSectionSize > arr.size())
            return false;

        QByteArray tmp((const char *)buf + usedBytes, arrSectionSize);
        decodeArrSection(tmp);
        usedBytes += arrSectionSize;
    }

    if ((configField.option() & EcgConfigField::OP_ArrConfig) != 0)
    {
        if (usedBytes + stSectionSize > arr.size())
            return false;

        QByteArray tmp((const char *)buf + usedBytes, stSectionSize);
        decodeArrSection(tmp);
        usedBytes += stSectionSize;
    }

    return true;
}

void CmsEcgConfigData::decodeArrSection(const QByteArray &rawData)
{
    if (rawData.size() != arrSectionSize)
        return;

    int index = 1;
    auto buf = (const unsigned char *)rawData.constData();
    for (int i = 0; i < CMS_ArrMax; i++, index++)
    {
        if(index <= 39)
        {
            configField.arrAlarmConfig[index].sw = GET_BIT(buf[i], 0);
            configField.arrAlarmConfig[index].level = (CmsAlarmLevel)(buf[i] >> 1);
        }
    }

    configField.pvcsLimit = buf[40];
    configField.stRiseLimit = (buf[41] << 0) + (buf[42] << 8);
    configField.stDepressedLimit = (buf[43] << 0) + (buf[44] << 8);
}

void CmsEcgConfigData::decodeStSection(const QByteArray &rawData)
{
    if (rawData.size() != stSectionSize)
        return;

    auto buf = (const unsigned char *)rawData.constData();
    configField.stAlarmConfig.sw = GET_BIT(buf[0], 0);
    configField.stAlarmConfig.level = (CmsAlarmLevel)(buf[0] >> 1);
    int index = 1;
    for (int i = 0; i < CMS_EcgLeadMax; i++, index += 4)
    {
        configField.stAlarmConfig.limit[i][CmsAlarmConfig<>::up_index] =
            (buf[index + 0] << 0) + (buf[index + 1] << 8);
        configField.stAlarmConfig.limit[i][CmsAlarmConfig<>::down_index] =
            (buf[index + 2] << 0) + (buf[index + 3] << 8);
    }
}

QByteArray CmsEcgConfigData::encodePacket() const
{
    char buf[baseSectionSize] = {0};

//    buf[4] = configField.workMode;

//    buf[5] = configField.leadType;

    switch (configField.workMode)
    {
        default:
        case CMS_EcgModeMonitor: buf[4] = 0; break;
        case CMS_EcgModeOperation: buf[4] = 1; break;
        case CMS_EcgModeDiangose: buf[4] = 2; break;
        case CMS_EcgModeSt: buf[4] = 3; break;
        case CMS_EcgModeUser: buf[4] = 4; break;
    }

    switch (configField.leadType)
    {
        default:
        case CMS_EcgType3: buf[5] = 3; break;
        case CMS_EcgType5: buf[5] = 5; break;
        case CMS_EcgType6: buf[5] = 6; break;
        case CMS_EcgType12: buf[5] = 12; break;
    }

    configField.arrSwitch ? SET_BIT(buf[6], 0) : CLR_BIT(buf[6], 0);
    configField.stSwitch ? SET_BIT(buf[6], 1) : CLR_BIT(buf[6], 1);
    configField.paceSwitch ? SET_BIT(buf[6], 2) : CLR_BIT(buf[6], 2);

    buf[7] = configField.monitorLead;

    configField.hrAlarmSetting.sw ? SET_BIT(buf[8], 0) : CLR_BIT(buf[8], 0);
    quint8 level = configField.hrAlarmSetting.level;
    buf[8] |= (level << 1);
    buf[9] = (configField.hrAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 0);
    buf[10] = (configField.hrAlarmSetting.limit[CmsAlarmConfig<>::up_index] >> 8);
    buf[11] = (configField.hrAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 0);
    buf[12] = (configField.hrAlarmSetting.limit[CmsAlarmConfig<>::down_index] >> 8);

    configField.paceSwitch ? SET_BIT(buf[6], 2) : CLR_BIT(buf[6], 2);

    buf[15] = configField.option();

    QByteArray content;
    content = QByteArray(buf + 4, sizeof(buf) - 4);
    if ((configField.option() & EcgConfigField::OP_ProbeInfo) != 0)
        content.append(encodeProbeInfo(configField.probeInfo));

    if ((configField.option() & EcgConfigField::OP_ArrConfig) != 0)
        content.append(encodeArrSection());

    if ((configField.option() & EcgConfigField::OP_StConfig) != 0)
        content.append(encodeStSection());

    return content;
}

QByteArray CmsEcgConfigData::encodeArrSection() const
{
    char buf[arrSectionSize] = {0};

    for (int i = 1, index = 0; i <= CMS_ArrMax; i++, index++)
    {
        configField.arrAlarmConfig[index].sw ? SET_BIT(buf[index], 0) : CLR_BIT(buf[index], 0);
        quint8 level = configField.arrAlarmConfig[index].level;
        buf[index] |= (level << 1);
    }

    buf[40] = configField.pvcsLimit;
    buf[41] = (configField.stRiseLimit >> 0);
    buf[42] = (configField.stRiseLimit >> 8);
    buf[43] = (configField.stDepressedLimit >> 0);
    buf[44] = (configField.stDepressedLimit >> 8);

    return QByteArray(buf, sizeof(buf));
}

QByteArray CmsEcgConfigData::encodeStSection() const
{
    char buf[53] = {0};

    configField.stAlarmConfig.sw  ? SET_BIT(buf[0], 0) : CLR_BIT(buf[0], 0);
    quint8 level = configField.stAlarmConfig.level;
    buf[0] |= (level << 1);

    for (int i = 0, index = 1; i < CMS_EcgLeadMax; i++, index += 4)
    {
        buf[index + 0] = (configField.stAlarmConfig.limit[i][CmsAlarmConfig<>::up_index] >> 0);
        buf[index + 1] = (configField.stAlarmConfig.limit[i][CmsAlarmConfig<>::up_index] >> 8);
        buf[index + 2] = (configField.stAlarmConfig.limit[i][CmsAlarmConfig<>::down_index] >> 0);
        buf[index + 3] = (configField.stAlarmConfig.limit[i][CmsAlarmConfig<>::down_index] >> 8);
    }

    return QByteArray(buf, sizeof(buf));
}


