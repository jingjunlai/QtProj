#include "cmsibpmoduledata.h"
#include <QDebug>

CmsIbpModuleData::CmsIbpModuleData(CmsModuleID identifier, CmsLabelID label)
    : CmsModuleData(identifier, label)
{

}

CmsIbpModuleData::~CmsIbpModuleData()
{

}

void CmsIbpModuleData::setData(IbpDataField data)
{
    this->dataField = data;
}

IbpDataField CmsIbpModuleData::data() const
{
    return this->dataField;
}

bool CmsIbpModuleData::decodePacket(const QByteArray &arr)
{
    if (arr.size() < paramSectionSize)
        return false;

    auto buf = (const unsigned char *)arr.constData();
    int usedBytes = 0;

    // 探头状态
    this->probeState = (CmsProbeState)(buf[5] & 0x07);

    // info
    dataField.sensorOff = GET_BIT(buf[6], 0) == 1;
    dataField.catheterOff = GET_BIT(buf[6], 1) == 1;
    dataField.zeroStatus = (buf[6] >> 2) & 0x03;
    dataField.calibrationStatus = (buf[6] >> 4) & 0x03;
    dataField.hasRawWave = GET_BIT(buf[6], 6) == 1;

    // param
    dataField.sys = (buf[7] << 0) + (buf[8] << 8) - 100 * 100;
    dataField.dia = (buf[9] << 0) + (buf[10] << 8) - 100 * 100;
    dataField.mean = (buf[11] << 0) + (buf[12] << 8) - 100 * 100;
    dataField.pulseRate = (buf[13] << 0) + (buf[14] << 8);
    dataField.ppv = (buf[15] << 0) + (buf[16] << 8) - 100 * 100;
    dataField.spv = (buf[17] << 0) + (buf[18] << 8) - 100 * 100;

    // wave section
    usedBytes += paramSectionSize;
    dataField.waves.clear();
    int waveCounts = (buf[usedBytes + 0] << 0) + (buf[usedBytes + 1] << 8);
    int blockSize = 3;
    int waveBytesLen = waveCounts * blockSize;
    usedBytes += 2;
    if (usedBytes + waveBytesLen > arr.size())
    {
        qWarning() << QString("decode ibp wave error. packet wave bytes: %1, rest bytes: %2")
                   .arg(waveBytesLen)
                   .arg(arr.size() - usedBytes);
        return false;
    }

    const unsigned char *p = buf + usedBytes;
    for (int i = 0; i < waveCounts; i++)
    {
        IbpWavePoint wave;
        wave.rWave = GET_BIT(p[0], 0) == 1;
        uchar tmp1 = p[1];
        uchar tmp2 = p[2];
        wave.ibpWave = ((tmp1 << 0) + (tmp2 << 8) - 20000) / 100 - 100;
//        if(wave.ibpWave > 25000)
//        {
//            QByteArray ba((char*)&tmp1, 1);
//            QByteArray ba2((char*)&tmp2, 1);
//            qDebug() << "tmp 1 = " << ba.toHex();
//            qDebug() << "tmp 2 = " << ba2.toHex();
//        }
        dataField.waves.append(wave);

        p += blockSize;
    }
    usedBytes += waveBytesLen;

    return true;
}

QByteArray CmsIbpModuleData::encodePacket() const
{
    char buf[paramSectionSize] = {0};

    // 探头状态
    buf[5] = this->probeState;

    // 信息
    dataField.sensorOff ? SET_BIT(buf[6], 0) : CLR_BIT(buf[6], 0);
    dataField.catheterOff ? SET_BIT(buf[6], 1) : CLR_BIT(buf[6], 1);

    dataField.zeroStatus ? SET_BIT(buf[6], 0) : CLR_BIT(buf[6], 0);
    dataField.catheterOff ? SET_BIT(buf[6], 1) : CLR_BIT(buf[6], 1);
    buf[6] |= ((dataField.zeroStatus & 0x03) << 2);
    buf[6] |= ((dataField.calibrationStatus & 0x03) << 4);
    dataField.hasRawWave ? SET_BIT(buf[6], 6) : CLR_BIT(buf[6], 6);

    // param
    quint16 sys = dataField.sys + 100 * 100;
    buf[7] = (sys >> 0) & 0xff;
    buf[8] = (sys >> 8) & 0xff;

    quint16 dia = dataField.dia + 100 * 100;
    buf[9] = (dia >> 0) & 0xff;
    buf[10] = (dia >> 8) & 0xff;

    quint16 mean = dataField.mean + 100 * 100;
    buf[11] = (mean >> 0) & 0xff;
    buf[12] = (mean >> 8) & 0xff;

    buf[13] = (dataField.pulseRate >> 0) & 0xff;
    buf[14] = (dataField.pulseRate >> 8) & 0xff;

    quint16 ppv = dataField.ppv + 100 * 100;
    buf[15] = (ppv >> 0) & 0xff;
    buf[16] = (ppv >> 8) & 0xff;

    quint16 spv = dataField.spv + 100 * 100;
    buf[15] = (spv >> 0) & 0xff;
    buf[16] = (spv >> 8) & 0xff;

    QByteArray content;
    content = QByteArray(buf + 4, sizeof(buf) - 4);
    content.append(encodeWaves());
    return content;
}

QByteArray CmsIbpModuleData::encodeWaves() const
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
        char buf[3] = {0};
        auto wavePoint = dataField.waves.at(i);
        quint16 wave = wavePoint.ibpWave + 200 * 100;
        buf[0] = wavePoint.rWave == true ? 1 : 0;
        buf[1] = (wave >> 0) & 0xff;
        buf[2] = (wave >> 8) & 0xff;

        tmp += QByteArray(buf, sizeof(buf));
    }

    tmp[0] = (char)(waveCounts >> 0);
    tmp[1] = (char)(waveCounts >> 8);

    return tmp;
}

