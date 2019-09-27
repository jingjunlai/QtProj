#ifndef CMSARRDATA_H
#define CMSARRDATA_H

#include "icmsdata.h"

struct ArrDataField
{
    quint8 Pvcs = 0;
};

class CmsArrData : public ICmsData
{
public:
    CmsArrData();

    virtual bool fromByteArray(const QByteArray &arr);

    virtual QByteArray toByteArray() const;

    ArrDataField data() const;

protected:

    virtual bool decodePacket(const QByteArray &arr);

    ArrDataField        m_Field;

};

Q_DECLARE_METATYPE(ArrDataField)

#endif // CMSARRDATA_H
