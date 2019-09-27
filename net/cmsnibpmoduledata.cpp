#include "cmsnibpmoduledata.h"
#include <QDebug>

CmsNibpModuleData::CmsNibpModuleData(CmsModuleID identifier, CmsLabelID label)
    : CmsModuleData(identifier, label)
{

}

CmsNibpModuleData::~CmsNibpModuleData()
{

}

void CmsNibpModuleData::setData(NibpDataField data)
{
    this->dataField = data;
}

NibpDataField CmsNibpModuleData::data() const
{
    return this->dataField;
}

bool CmsNibpModuleData::decodePacket(const QByteArray &arr)
{
    if (arr.size() < paramSectionSize)
        return false;

    auto buf = (const unsigned char *)arr.constData();
    int usedBytes = 0;

    // 探头状态
    this->probeState = (CmsProbeState)(buf[5] & 0x07);

    // 工作模式
    dataField.workMode = (CmsNibpWorkMode)(buf[6] & 0x0f);

    // stauts
    dataField.working = GET_BIT(buf[7], 0) == 1;
    dataField.hasResult = GET_BIT(buf[7], 1) == 1;
    dataField.newResult = GET_BIT(buf[7], 2) == 1;
    dataField.hasError = GET_BIT(buf[7], 3) == 1;

    // 错误代码
    dataField.errorCode = (CmsNibpErrorID)buf[8];

    // value
    dataField.sys  = (buf[13] << 0) + (buf[14] << 8);
    dataField.dia  = (buf[15] << 0) + (buf[16] << 8);
    dataField.map = (buf[17] << 0) + (buf[18] << 8);
    dataField.pr   = (buf[19] << 0) + (buf[20] << 8);

    // wave section
    usedBytes += paramSectionSize;
    dataField.waves.clear();

    return true;
}

QByteArray CmsNibpModuleData::encodePacket() const
{
    char buf[paramSectionSize] = {0};

    // 探头状态
    buf[5] = this->probeState;

    // 工作模式
    buf[6] = dataField.workMode;

    // stauts
    dataField.working ? SET_BIT(buf[7], 0) : CLR_BIT(buf[7], 0);
    dataField.hasResult ? SET_BIT(buf[7], 1) : CLR_BIT(buf[7], 1);
    dataField.newResult ? SET_BIT(buf[7], 2) : CLR_BIT(buf[7], 2);
    dataField.hasError ? SET_BIT(buf[7], 3) : CLR_BIT(buf[7], 3);

    // 错误代码
    buf[8] = dataField.errorCode;

    buf[13] = (dataField.sys >> 0) & 0xff;
    buf[14] = (dataField.sys >> 8) & 0xff;

    buf[15] = (dataField.dia >> 0) & 0xff;
    buf[16] = (dataField.dia >> 8) & 0xff;

    buf[17] = (dataField.map >> 0) & 0xff;
    buf[18] = (dataField.map >> 8) & 0xff;

    buf[19] = (dataField.pr >> 0) & 0xff;
    buf[20] = (dataField.pr >> 8) & 0xff;

    QByteArray content;
    content = QByteArray(buf + 4, sizeof(buf) - 4);
    content.append(encodeWaves());
    return content;
}

QByteArray CmsNibpModuleData::encodeWaves() const
{
    QByteArray tmp;
    tmp.append((char)0);
    tmp.append((char)0);
    if (dataField.waves.isEmpty())
        return tmp;

    int waveCounts = dataField.waves.size();
    for (int i = 0; i < waveCounts; i++)
    {
        char buf[2] = {0};
        auto w = dataField.waves.at(i);
        quint16 v = w.wave * 100;
        buf[0] = (v >> 8)& 0xff;
        buf[1] = (v >> 0)& 0xff;

        tmp += QByteArray(buf, sizeof(buf));
    }

    tmp[0] = (char)(waveCounts >> 0);
    tmp[1] = (char)(waveCounts >> 8);

    return tmp;
}


