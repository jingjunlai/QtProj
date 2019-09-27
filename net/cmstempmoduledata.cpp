#include "cmstempmoduledata.h"
#include <QDebug>

CmsTempModuleData::CmsTempModuleData(CmsModuleID identifier, CmsLabelID label)
    : CmsModuleData(identifier, label)
{

}

CmsTempModuleData::~CmsTempModuleData()
{

}

void CmsTempModuleData::setData(TempDataField data)
{
    this->dataField = data;
}

TempDataField CmsTempModuleData::data() const
{
    return this->dataField;
}

bool CmsTempModuleData::decodePacket(const QByteArray &arr)
{
    if (arr.size() < paramSectionSize)
        return false;

    auto buf = (const unsigned char *)arr.constData();
    int usedBytes = 0;

    dataField.isDualTemp = GET_BIT(buf[5], 0) == 1;

    // 体温1
    probeState = (CmsProbeState)(buf[6] & 0x07);
    dataField.temp[0].subid = buf[7];
    dataField.temp[0].measureStatus = buf[8];
    dataField.temp[0].data = (buf[9] << 0) + (buf[10] << 8);

    // 体温2
    probeState2 = (CmsProbeState)(buf[11] & 0x07);
    dataField.temp[1].subid = buf[12];
    dataField.temp[1].measureStatus = buf[13];
    dataField.temp[1].data = (buf[14] << 0) + (buf[15] << 8);

    usedBytes += paramSectionSize;

    return true;
}

QByteArray CmsTempModuleData::encodePacket() const
{
    char buf[paramSectionSize] = {0};

    // 单双体温
    dataField.isDualTemp ? SET_BIT(buf[5], 0) : CLR_BIT(buf[5], 0);

    // 体温1
    buf[6] = probeState;
    buf[7] = dataField.temp[0].subid;
    buf[8] = dataField.temp[0].measureStatus;
    buf[9] = dataField.temp[0].data >> 0;
    buf[10] = dataField.temp[0].data >> 8;

    // 体温2
    buf[11] = probeState2;
    buf[12] = dataField.temp[1].subid;
    buf[13] = dataField.temp[1].measureStatus;
    buf[14] = dataField.temp[1].data >> 0;
    buf[15] = dataField.temp[1].data >> 8;

    QByteArray content;
    content = QByteArray(buf + 4, sizeof(buf) - 4);
    return content;
}

