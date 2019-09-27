#include "cmscomoduledata.h"
#include <QDebug>

CmsCoModuleData::CmsCoModuleData(CmsModuleID identifier, CmsLabelID label)
    : CmsModuleData(identifier, label)
{

}

CmsCoModuleData::~CmsCoModuleData()
{

}

void CmsCoModuleData::setData(CoDataField data)
{
    this->dataField = data;
}

CoDataField CmsCoModuleData::data() const
{
    return this->dataField;
}

bool CmsCoModuleData::decodePacket(const QByteArray &arr)
{
    if (arr.size() < paramSectionSize)
        return false;

    auto buf = (const unsigned char *)arr.constData();
    int usedBytes = 0;

    // 探头状态
    this->probeState = (CmsProbeState)(buf[5] & 0x07);

    // info
//    dataField.working = GET_BIT(buf[6], 0) == 1;
//    dataField.etValid= GET_BIT(buf[6], 1) == 1;
//    dataField.fiValid = GET_BIT(buf[6], 2) == 1;
//    dataField.awrrValid = GET_BIT(buf[6], 3) == 1;

//    // param
//    dataField.etValue = (buf[7] << 0) + (buf[8] << 8);
//    dataField.fiValue = (buf[9] << 0) + (buf[10] << 8);
//    dataField.awrrValue = buf[11];

    // wave section
    usedBytes += paramSectionSize;
//    dataField.waves.clear();
    int waveCounts = (buf[usedBytes + 0] << 0) + (buf[usedBytes + 1] << 8);
    int blockSize = 2;
    int waveBytesLen = waveCounts * blockSize;
    usedBytes += 2;
    if (usedBytes + waveBytesLen > arr.size())
    {
        qWarning() << QString("decode co wave error. need packet wave bytes: %1, rest bytes: %2")
                   .arg(waveBytesLen)
                   .arg(arr.size() - usedBytes);
        return false;
    }

//    const unsigned char *p = buf + usedBytes;
//    for (int i = 0; i < waveCounts; i++)
//    {
//        Co2WavePoint wave;
//        wave.co2Wave = (p[0] << 0) + (p[1] << 8);
//        dataField.waves.append(wave);

//        p += blockSize;
//    }
//    usedBytes += waveBytesLen;

    return true;
}

QByteArray CmsCoModuleData::encodePacket() const
{
    char buf[paramSectionSize] = {0};

    // 探头状态
    buf[5] = this->probeState;

    // 信息
//    dataField.working ? SET_BIT(buf[6], 0) : CLR_BIT(buf[6], 0);
//    dataField.etValid ? SET_BIT(buf[6], 1) : CLR_BIT(buf[6], 1);
//    dataField.fiValid ? SET_BIT(buf[6], 2) : CLR_BIT(buf[6], 2);
//    dataField.awrrValid ? SET_BIT(buf[6], 3) : CLR_BIT(buf[6], 3);

//    // param
//    buf[7] = (dataField.etValue >> 0) & 0xff;
//    buf[8] = (dataField.etValue >> 8) & 0xff;

//    buf[9] = (dataField.fiValue >> 0) & 0xff;
//    buf[10] = (dataField.fiValue >> 8) & 0xff;

//    buf[11] = dataField.awrrValue;

    QByteArray content;
    content = QByteArray(buf + 4, sizeof(buf) - 4);
//    content.append(encodeWaves());
    return content;
}

