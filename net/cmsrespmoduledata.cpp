#include "cmsrespmoduledata.h"
#include <QDebug>

CmsRespModuleData::CmsRespModuleData(CmsModuleID identifier, CmsLabelID label)
    : CmsModuleData(identifier, label)
{

}

CmsRespModuleData::~CmsRespModuleData()
{

}

void CmsRespModuleData::setData(RespDataField data)
{
    this->dataField = data;
}

RespDataField CmsRespModuleData::data() const
{
    return this->dataField;
}

bool CmsRespModuleData::decodePacket(const QByteArray &arr)
{
    if (arr.size() < paramSectionSize)
        return false;

    auto buf = (const unsigned char *)arr.constData();
    int usedBytes = 0;

    // 探头状态
    this->probeState = (CmsProbeState)(buf[5] & 0x07);

    // info
    dataField.sensorOff = GET_BIT(buf[6], 0) == 1;
    dataField.notUpdated = GET_BIT(buf[6], 1) == 1;
    dataField.apnea = GET_BIT(buf[6], 2) == 1;
    dataField.cardiacInterference = GET_BIT(buf[7], 3) == 1;

    dataField.calcLead = buf[7] & 0x01;
    dataField.respRate = buf[8];

    // wave section
    usedBytes += paramSectionSize;
    dataField.waves.clear();
    int waveCounts = (buf[usedBytes + 0] << 0) + (buf[usedBytes + 1] << 8);
    int blockSize = 3;
    int waveBytesLen = waveCounts * blockSize;
    usedBytes += 2;
    if (usedBytes + waveBytesLen > arr.size())
    {
        qWarning() << QString("decode resp wave error. need packet wave bytes: %1, rest bytes: %2")
                   .arg(waveBytesLen)
                   .arg(arr.size() - usedBytes);
        return false;
    }

    const unsigned char *p = buf + usedBytes;
    for (int i = 0; i < waveCounts; i++)
    {
        RespWavePoint wave;
        switch (p[0] & 0x03)
        {
            default:
            case 0:
                wave.phase = RESP_Normal;
                break;
            case 1:
                wave.phase = RESP_Inspiration;
                break;
            case 2:
                wave.phase = RESP_Expiration;
                break;
        }
        quint16 v = (p[1] << 0) + (p[2] << 8);
        wave.respWave = v + INT16_MIN;
        dataField.waves.append(wave);

        p += blockSize;
    }
    usedBytes += waveBytesLen;

    return true;
}

QByteArray CmsRespModuleData::encodePacket() const
{
    char buf[paramSectionSize] = {0};

    // 探头状态
    buf[5] = this->probeState;

    // info
    dataField.sensorOff ? SET_BIT(buf[6], 0) : CLR_BIT(buf[6], 0);
    dataField.notUpdated ? SET_BIT(buf[6], 1) : CLR_BIT(buf[6], 1);
    dataField.apnea ? SET_BIT(buf[6], 2) : CLR_BIT(buf[6], 2);
    dataField.cardiacInterference ? SET_BIT(buf[6], 3) : CLR_BIT(buf[6], 3);

    buf[7] = dataField.calcLead & 0x01;
    buf[8] = dataField.respRate;

    QByteArray content;
    content = QByteArray(buf + 4, sizeof(buf) - 4);
    content.append(encodeWaves());
    return content;
}

QByteArray CmsRespModuleData::encodeWaves() const
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
        switch (wave.phase)
        {
            default:
            case RESP_Normal:
                break;
            case RESP_Inspiration:
                SET_BIT(buf[0], 0);
                break;
            case RESP_Expiration:
                SET_BIT(buf[0], 1);
                break;
        }

        quint16 v = wave.respWave - INT16_MIN;
        buf[1] = (v >> 0) & 0xff;
        buf[2] = (v >> 8) & 0xff;

        tmp += QByteArray(buf, sizeof(buf));
    }

    tmp[0] = (char)(waveCounts >> 0);
    tmp[1] = (char)(waveCounts >> 8);

    return tmp;
}

