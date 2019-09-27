#ifndef CMSCOMMANDDATA_H
#define CMSCOMMANDDATA_H


#include "icmsdata.h"

class CmsCommandData : public ICmsData
{
public:
    CmsCommandData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsCommandData();

    virtual bool fromByteArray(const QByteArray &arr);

    virtual QByteArray toByteArray() const;

protected:
    virtual bool decodePacket(const QByteArray &arr) = 0;

    virtual QByteArray encodePacket() const = 0;

protected:

    CmsModuleID             identifier = CMS_ModuleNull;
    CmsLabelID              label = CMS_LabelNull;
//    CmsCommandType          mCommandNumber = CMS_CommandUnknown;
//    quint8                  mLocalIdentifer = 0;

};

#endif // CMSCOMMANDDATA_H
