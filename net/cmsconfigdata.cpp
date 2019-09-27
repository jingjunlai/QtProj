#include "cmsconfigdata.h"

CmsConfigData::CmsConfigData(CmsModuleID identifierid, CmsLabelID labelid)
    : ICmsData()
{
    this->identifier = identifierid;
    this->label = labelid;
}

CmsConfigData::~CmsConfigData()
{
}

bool CmsConfigData::fromByteArray(const QByteArray &arr)
{
    identifier = CMS_ModuleNull;
    label = CMS_LabelNull;

    if (arr.size() < 4)
        return false;

    int len = (arr.at(2) << 0) + (arr.at(3) << 8);
    if (len + 4 < arr.size())
        return false;

    identifier = (CmsModuleID)arr.at(0);
    label = (CmsLabelID)arr.at(1);

    return decodePacket(arr);
}

QByteArray CmsConfigData::toByteArray() const
{
    QByteArray content = encodePacket();
    quint16 len = content.size();

    QByteArray arr;
    arr.append((char)identifier);
    arr.append((char)label);
    arr.append((char)(len >> 0));
    arr.append((char)(len >> 8));
    arr.append(content);

    return arr;
}

void CmsConfigData::decodeProbeInfo(QByteArray rawData, CmsProbeInfo *info) const
{
    if (info == Q_NULLPTR || rawData.size() != probeSectionSize)
        return;

    memcpy(info, rawData.constData(), probeSectionSize);
}

QByteArray CmsConfigData::encodeProbeInfo(CmsProbeInfo info) const
{
    char buf[probeSectionSize] = {0};
    memcpy(buf, &info, sizeof(buf));
    return QByteArray(buf, sizeof(buf));
}

