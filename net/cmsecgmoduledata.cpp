#include "cmsecgmoduledata.h"
#include <QDebug>



#define ECG_WAVE_OFFSET        30000


CmsEcgModuleData::CmsEcgModuleData(CmsModuleID identifier, CmsLabelID label)
    : CmsModuleData(identifier, label)
{

}

CmsEcgModuleData::~CmsEcgModuleData()
{

}

void CmsEcgModuleData::setData(EcgDataField data)
{
    this->dataField = data;
}

EcgDataField CmsEcgModuleData::data() const
{
    return this->dataField;
}

bool CmsEcgModuleData::decodePacket(const QByteArray &arr)
{
    if (arr.size() < paramSectionSize)
        return false;

    auto buf = (const unsigned char *)arr.constData();
    int usedBytes = 0;

    // 探头状态
    this->probeState = (CmsProbeState)(buf[5] & 0x07);

    // 导联类型
    switch (buf[6] & 0x0f)
    {
        case 3:
            dataField.leadType = CMS_EcgType3;
            break;
        case 5:
            dataField.leadType = CMS_EcgType5;
            break;
        case 6:
            dataField.leadType = CMS_EcgType6;
            break;
        case 12:
            dataField.leadType = CMS_EcgType12;
            break;
        default:
            {
                dataField.leadType = CMS_EcgType12;
                qDebug() << "decode ecg lead type error";
            }
            break;
    }

    // 计算导联
    dataField.calcLeadIndex = (CmsECGLeadIndex)(buf[7] & 0x0f);

    // 脱落信息
    dataField.electrodeOff[CMS_EcgElecRa] = GET_BIT(buf[8], 0) == 1;
    dataField.electrodeOff[CMS_EcgElecRl] = GET_BIT(buf[8], 1) == 1;
    dataField.electrodeOff[CMS_EcgElecLa] = GET_BIT(buf[8], 2) == 1;
    dataField.electrodeOff[CMS_EcgElecLl] = GET_BIT(buf[8], 3) == 1;

    dataField.electrodeOff[CMS_EcgElecC1] = GET_BIT(buf[9], 0) == 1;
    dataField.electrodeOff[CMS_EcgElecC2] = GET_BIT(buf[9], 1) == 1;
    dataField.electrodeOff[CMS_EcgElecC3] = GET_BIT(buf[9], 3) == 1;
    dataField.electrodeOff[CMS_EcgElecC4] = GET_BIT(buf[9], 4) == 1;
    dataField.electrodeOff[CMS_EcgElecC5] = GET_BIT(buf[9], 5) == 1;
    dataField.electrodeOff[CMS_EcgElecC6] = GET_BIT(buf[9], 6) == 1;

    // 极化信息
    dataField.polarization[CMS_EcgLeadI] = GET_BIT(buf[10], 0) == 1;
    dataField.polarization[CMS_EcgLeadII] = GET_BIT(buf[10], 1) == 1;
    dataField.polarization[CMS_EcgLeadV1] = GET_BIT(buf[10], 2) == 1;
    dataField.polarization[CMS_EcgLeadV2] = GET_BIT(buf[10], 3) == 1;
    dataField.polarization[CMS_EcgLeadV3] = GET_BIT(buf[10], 4) == 1;
    dataField.polarization[CMS_EcgLeadV4] = GET_BIT(buf[10], 5) == 1;
    dataField.polarization[CMS_EcgLeadV5] = GET_BIT(buf[10], 6) == 1;
    dataField.polarization[CMS_EcgLeadV6] = GET_BIT(buf[10], 7) == 1;

    // hr
    dataField.hr = (buf[17] << 0) + (buf[18] << 8);
    if (dataField.hr > 300)
        dataField.hr = 65535;
    usedBytes += paramSectionSize;

    // wave section
    dataField.waves.clear();
    int waveCounts = (buf[usedBytes + 0] << 0) + (buf[usedBytes + 1] << 8);
    int blockSize = 17;
    int waveBytesLen = waveCounts * blockSize;
    usedBytes += 2;
    if (usedBytes + waveBytesLen > arr.size())
    {
        qWarning() << QString("decode ecg wave error. need packet wave bytes: %1, rest bytes: %2")
                   .arg(waveBytesLen)
                   .arg(arr.size() - usedBytes);
        return false;
    }

    const unsigned char *p = buf + usedBytes;
    for (int i = 0; i < waveCounts; i++)
    {
        EcgWavePoint wave;
        wave.rwave = GET_BIT(p[0], 0) == 1;
        wave.pace = GET_BIT(p[0], 1) == 1;

        for (int j = 0; j < 8; j++)
        {
            // 双字节
            int value = (p[1 + 2 * j + 0] << 0) + (p[1 + 2 * j + 1] << 8);
            wave.wave[j] = (value - ECG_WAVE_OFFSET);// * 10    BY LJJ 2019.5.9;
        }
        dataField.waves.append(wave);

        p += blockSize;
    }
    usedBytes += waveBytesLen;

    // raw wave section
    if (!dataField.hasRawWaves)
        return true;

    int rawWavesByteLen = (buf[usedBytes + 0] << 0) + (buf[usedBytes + 1] << 8);
    usedBytes += 2;
    if (rawWavesByteLen + usedBytes > arr.size())
    {
        qWarning() << QString("decode ecg wave error. packet raw wave bytes: %1, rest bytes: %2")
                   .arg(waveBytesLen)
                   .arg(arr.size() - rawWavesByteLen - usedBytes);
        return false;
    }

    int rawWaveCounts = rawWavesByteLen / 25;
    p = buf + usedBytes;
    for (int i = 0; i < rawWaveCounts; i++)
    {
        EcgWavePoint wave;
        wave.rwave = GET_BIT(p[0], 0) == 1;
        wave.pace = GET_BIT(p[0], 1) == 1;

        for (int j = 0; j < 8; j++)
        {
            // 三字节
            int value = (p[1 + 2 * j + 0] << 0) +
                        (p[1 + 2 * j + 1] << 8) +
                        (p[1 + 2 * j + 2] << 16);
            wave.wave[j] = (value - ECG_WAVE_OFFSET);// * 10;   BY LJJ 2019.5.9
        }
        dataField.rawWaves.append(wave);

        p += 25;
    }

    return true;
}

QByteArray CmsEcgModuleData::encodePacket() const
{
    char buf[paramSectionSize] = {0};

    // 探头状态
    buf[5] = this->probeState;

    // 导联类型
    switch (dataField.leadType)
    {
        default:
        case CMS_EcgType3: buf[6] = 3; break;
        case CMS_EcgType5: buf[6] = 5; break;
        case CMS_EcgType6: buf[6] = 6; break;
        case CMS_EcgType12: buf[6] = 12; break;
    }

    // 计算导联
    buf[7] = dataField.calcLeadIndex;

    // 脱落信息
    dataField.electrodeOff[CMS_EcgElecRa] ? SET_BIT(buf[8], 0) : CLR_BIT(buf[8], 0);
    dataField.electrodeOff[CMS_EcgElecRl] ? SET_BIT(buf[8], 1) : CLR_BIT(buf[8], 1);
    dataField.electrodeOff[CMS_EcgElecLa] ? SET_BIT(buf[8], 2) : CLR_BIT(buf[8], 2);
    dataField.electrodeOff[CMS_EcgElecLl] ? SET_BIT(buf[8], 3) : CLR_BIT(buf[8], 3);

    dataField.electrodeOff[CMS_EcgElecC1] ? SET_BIT(buf[9], 0) : CLR_BIT(buf[9], 0);
    dataField.electrodeOff[CMS_EcgElecC2] ? SET_BIT(buf[9], 1) : CLR_BIT(buf[9], 1);
    dataField.electrodeOff[CMS_EcgElecC3] ? SET_BIT(buf[9], 2) : CLR_BIT(buf[9], 2);
    dataField.electrodeOff[CMS_EcgElecC4] ? SET_BIT(buf[9], 3) : CLR_BIT(buf[9], 3);
    dataField.electrodeOff[CMS_EcgElecC5] ? SET_BIT(buf[9], 4) : CLR_BIT(buf[9], 4);
    dataField.electrodeOff[CMS_EcgElecC6] ? SET_BIT(buf[9], 5) : CLR_BIT(buf[9], 5);
    dataField.hasRawWaves ? SET_BIT(buf[9], 6) : CLR_BIT(buf[9], 6);

    // bytep 9 ~ 15 保留的

    // hr
    buf[17] = (dataField.hr >> 0) & 0xff;
    buf[18] = (dataField.hr >> 8) & 0xff;

    QByteArray content;
    content = QByteArray(buf + 4, sizeof(buf) - 4);
    content.append(encodeWaves());
    content.append(encodeRawWaves());
    return content;
}

QByteArray CmsEcgModuleData::encodeWaves() const
{
    QByteArray tmp;
    tmp.append((char)0);
    tmp.append((char)0);
    if (dataField.waves.isEmpty())
        return tmp;

    int waveCounts = dataField.waves.size();
//    waveCounts = 0;
    for (int i = 0; i < waveCounts; i++)
    {
        char buf[17] = {0}; // 17 = 1 + 2 * 8
        auto wave = dataField.waves.at(i);
        wave.rwave ? SET_BIT(buf[0], 0) : CLR_BIT(buf[0], 0);
        wave.pace ? SET_BIT(buf[0], 1) : CLR_BIT(buf[0], 1);

        for (int j = 0; j < 8; j++)
        {
            quint16 value;
            int v = wave.wave[j] / 10;
            v = qBound(-ECG_WAVE_OFFSET, v, ECG_WAVE_OFFSET);
            value = v + ECG_WAVE_OFFSET;

            buf[1 + j * 2 + 0] = (value >> 0) & 0xff;
            buf[1 + j * 2 + 1] = (value >> 8) & 0xff;
        }

        tmp += QByteArray(buf, sizeof(buf));
    }

    tmp[0] = (char)(waveCounts >> 0);
    tmp[1] = (char)(waveCounts >> 8);

    return tmp;
}

QByteArray CmsEcgModuleData::encodeRawWaves() const
{
    if (dataField.rawWaves.isEmpty())
        return QByteArray();

    QByteArray tmp;
    tmp.append((char)0);
    tmp.append((char)0);
    for (int i = 0; i < dataField.rawWaves.size(); i++)
    {
        char buf[25] = {0}; // 25 = 1 + 3 * 8
        auto wave = dataField.rawWaves.at(i);
        wave.rwave ? SET_BIT(buf[0], 0) : CLR_BIT(buf[0], 0);
        wave.pace ? SET_BIT(buf[0], 1) : CLR_BIT(buf[0], 1);

        for (int j = 0; j < 8; j++)
        {
            quint32 value = wave.wave[j];

            buf[1 + j * 2 + 0] = (value >> 0) & 0xff;
            buf[1 + j * 2 + 1] = (value >> 8) & 0xff;
            buf[1 + j * 2 + 2] = (value >> 16) & 0xff;
        }

        tmp += QByteArray(buf, sizeof(buf));
    }

    quint16 count = dataField.rawWaves.size();
    tmp[0] = (char)(count >> 0);
    tmp[1] = (char)(count >> 8);

    return tmp;
}

