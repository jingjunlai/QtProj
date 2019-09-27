#ifndef CMSCOMODULEDATA_H
#define CMSCOMODULEDATA_H

#include "cmsmoduledata.h"

struct CoDataField
{
};

class CmsCoModuleData : public CmsModuleData
{
public:
    CmsCoModuleData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsCoModuleData();

    void setData(CoDataField data);

    CoDataField data() const;

protected:
    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

protected:
    CoDataField dataField;
    static const int paramSectionSize = 24;
};

Q_DECLARE_METATYPE(CoDataField)

#endif // CMSCOMODULEDATA_H
