#include "cmsmoduledata.h"

CmsModuleData::CmsModuleData(CmsModuleID identifierid, CmsLabelID labelid)
    : ICmsData()
{
    this->identifier = identifierid;
    this->label = labelid;
}

CmsModuleData::~CmsModuleData()
{

}

bool CmsModuleData::fromByteArray(const QByteArray &arr)
{
    identifier = CMS_ModuleNull;
    label = CMS_LabelNull;

    if (arr.size() < 4)
        return false;

    int len = (arr.at(2) << 0) + (arr.at(3) << 8);
    if (len + 4 > arr.size())
        return false;

    identifier = (CmsModuleID)arr.at(0);
    label = (CmsLabelID)arr.at(1);

    return decodePacket(arr);
}

QByteArray CmsModuleData::toByteArray() const
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

CmsModuleID CmsModuleData::cmsModuleId() const
{
    return identifier;
}

CmsLabelID CmsModuleData::cmsLabelId() const
{
    return label;
}

