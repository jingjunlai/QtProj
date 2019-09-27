#include "cmscommanddata.h"

CmsCommandData::CmsCommandData(CmsModuleID identifier, CmsLabelID label)
    : ICmsData()
{
    this->identifier = identifier;
    this->label = label;
}

CmsCommandData::~CmsCommandData()
{

}

bool CmsCommandData::fromByteArray(const QByteArray &arr)
{
//    mCommandNumber = CMS_CommandUnknown;
//    mLocalIdentifer = 0;

    identifier = CMS_ModuleNull;
    label = CMS_LabelNull;

    if (arr.size() < 4)
        return false;

    identifier = (CmsModuleID)arr.at(0);
    label = (CmsLabelID)arr.at(1);

//    mCommandNumber = (CmsCommandType)arr.at(2);
//    mLocalIdentifer = arr.at(3);

    return decodePacket(arr);
}

QByteArray CmsCommandData::toByteArray() const
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

