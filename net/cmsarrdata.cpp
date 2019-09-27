#include "cmsarrdata.h"

CmsArrData::CmsArrData()
    :ICmsData()
{

}

bool CmsArrData::fromByteArray(const QByteArray &arr)
{

    if (arr.size() < 2)
        return false;

    int len = (arr.at(0) << 0) + (arr.at(1) << 8);
    if (len + 2 > arr.size())
        return false;

    return decodePacket(arr);
}

QByteArray CmsArrData::toByteArray() const
{
    QByteArray ba = "";
    return ba;
}

ArrDataField CmsArrData::data() const
{
    return this->m_Field;
}

bool CmsArrData::decodePacket(const QByteArray &arr)
{
    m_Field.Pvcs = arr.at(29);
    return true;
}

