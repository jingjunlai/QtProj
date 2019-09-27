#include "cmsspo2moduledata.h"
#include <QDebug>

CmsSpo2ModuleData::CmsSpo2ModuleData(CmsModuleID identifier, CmsLabelID label)
    : CmsModuleData(identifier, label)
{

}

CmsSpo2ModuleData::~CmsSpo2ModuleData()
{

}

void CmsSpo2ModuleData::setData(Spo2DataField data)
{
    this->dataField = data;
}

Spo2DataField CmsSpo2ModuleData::data() const
{
    return this->dataField;
}

bool CmsSpo2ModuleData::decodePacket(const QByteArray &arr)
{
    if (arr.size() < paramSectionSize)
        return false;

    auto buf = (const unsigned char *)arr.constData();
    int usedBytes = 0;

    // 探头状态
    this->probeState = (CmsProbeState)(buf[5] & 0x07);

    // 信息
    dataField.sensorOff = GET_BIT(buf[6], 0) == 1;
    dataField.cableOff = GET_BIT(buf[6], 1) == 1;
    dataField.weakPulse = GET_BIT(buf[6], 2) == 1;
    dataField.searching = GET_BIT(buf[6], 3) == 1;
    dataField.interference = GET_BIT(buf[6], 4) == 1;

    dataField.resp.support = GET_BIT(buf[7], 0) == 1;
    dataField.resp.enable = GET_BIT(buf[7], 1) == 1;
    dataField.resp.status = GET_BIT(buf[7], 2) == 1;
    dataField.rainbow.support = GET_BIT(buf[7], 3) == 1;
    dataField.hasRawWave = GET_BIT(buf[7], 4) == 1;

    // data
    dataField.sqi = buf[8];
    dataField.spo2 = buf[9];
    dataField.pi = (buf[10] << 0) + (buf[11] << 8);
    dataField.pulseRate = (buf[12] << 0) + (buf[13] << 8);
    dataField.resp.respRate = buf[14];
    dataField.rainbow.spco = (buf[15] << 0) + (buf[16] << 8);
    dataField.rainbow.spmet = (buf[17] << 0) + (buf[18] << 8);
    dataField.rainbow.sphb = buf[19];
    dataField.rainbow.spoc = (buf[20] << 0) + (buf[21] << 8);
    dataField.rainbow.destIndex = buf[22];
    dataField.rainbow.deltaPi = (buf[23] << 9) + (buf[24] << 8);
    dataField.rainbow.pvi = buf[25];

    // wave section
    usedBytes += paramSectionSize;
    dataField.waves.clear();
    int waveCounts = (buf[usedBytes + 0] << 0) + (buf[usedBytes + 1] << 8);
    int blockSize = 3;
    int waveBytesLen = waveCounts * blockSize;
    usedBytes += 2;
    if (usedBytes + waveBytesLen > arr.size())
    {
        qWarning() << QString("decode spo2 wave error. need packet wave bytes: %1, rest bytes: %2")
                   .arg(waveBytesLen)
                   .arg(arr.size() - usedBytes);
        return false;
    }

    const unsigned char *p = buf + usedBytes;
    for (int i = 0; i < waveCounts; i++)
    {
        Spo2WavePoint wave;
        wave.bloodStrength = p[0] & 0x0f;
        wave.rwave = GET_BIT(p[0], 4) == 1;
        wave.spo2Wave = p[1];
        wave.respWave = p[2];
        dataField.waves.append(wave);

        p += blockSize;
    }
    usedBytes += waveBytesLen;

    // raw wave section

    return true;
}

QByteArray CmsSpo2ModuleData::encodePacket() const
{
    char buf[paramSectionSize] = {0};

    // 探头状态
    buf[5] = this->probeState;

    // 信息
    dataField.sensorOff ? SET_BIT(buf[6], 0) : CLR_BIT(buf[6], 0);
    dataField.cableOff ? SET_BIT(buf[6], 1) : CLR_BIT(buf[6], 1);
    dataField.weakPulse ? SET_BIT(buf[6], 2) : CLR_BIT(buf[6], 2);
    dataField.searching ? SET_BIT(buf[6], 3) : CLR_BIT(buf[6], 3);
    dataField.interference ? SET_BIT(buf[6], 4) : CLR_BIT(buf[6], 4);

    dataField.resp.support ? SET_BIT(buf[7], 0) : CLR_BIT(buf[7], 0);
    dataField.resp.enable ? SET_BIT(buf[7], 1) : CLR_BIT(buf[7], 1);
    dataField.resp.status ? SET_BIT(buf[7], 2) : CLR_BIT(buf[7], 2);
    dataField.rainbow.support ? SET_BIT(buf[7], 3) : CLR_BIT(buf[7], 3);
    dataField.hasRawWave ? SET_BIT(buf[7], 4) : CLR_BIT(buf[7], 4);

    // data
    buf[8] = dataField.sqi;
    buf[9] = dataField.spo2;

    buf[10] = (dataField.pi >> 0) & 0xff;
    buf[11] = (dataField.pi >> 8) & 0xff;

    buf[12] = (dataField.pulseRate >> 0) & 0xff;
    buf[13] = (dataField.pulseRate >> 8) & 0xff;

    buf[14] = dataField.resp.respRate;

    buf[15] = (dataField.rainbow.spco >> 0) & 0xff;
    buf[16] = (dataField.rainbow.spco >> 8) & 0xff;

    buf[17] = (dataField.rainbow.spmet >> 0) & 0xff;
    buf[18] = (dataField.rainbow.spmet >> 8) & 0xff;

    buf[19] = dataField.rainbow.sphb;

    buf[20] = (dataField.rainbow.spoc >> 0) & 0xff;
    buf[21] = (dataField.rainbow.spoc >> 8) & 0xff;

    buf[22] = dataField.rainbow.destIndex;

    buf[23] = (dataField.rainbow.deltaPi >> 0) & 0xff;
    buf[24] = (dataField.rainbow.deltaPi >> 8) & 0xff;

    buf[25] = dataField.rainbow.pvi;

    QByteArray content;
    content = QByteArray(buf + 4, sizeof(buf) - 4);
    content.append(encodeWaves());
    content.append(encodeRawWaves());
    return content;
}

QByteArray CmsSpo2ModuleData::encodeWaves() const
{
    QByteArray tmp;
    tmp.append((char)0);
    tmp.append((char)0);
    if (dataField.waves.isEmpty())
        return tmp;

    int waveCounts = dataField.waves.size();
    //waveCounts = 0;
    for (int i = 0; i < waveCounts; i++)
    {
        char buf[3] = {0};
        auto wave = dataField.waves.at(i);
        buf[0] = wave.bloodStrength & 0x0f;
        wave.rwave ? SET_BIT(buf[0], 4) : CLR_BIT(buf[0], 4);
        buf[1] = wave.spo2Wave & 0xff;
        buf[2] = wave.respWave & 0xff;

        tmp += QByteArray(buf, sizeof(buf));
    }

    tmp[0] = (char)(waveCounts >> 0);
    tmp[1] = (char)(waveCounts >> 8);

    return tmp;
}

QByteArray CmsSpo2ModuleData::encodeRawWaves() const
{
    if (dataField.waves.isEmpty())
        return QByteArray();

    return QByteArray();
}

