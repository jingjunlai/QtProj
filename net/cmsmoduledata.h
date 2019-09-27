#ifndef CMSMODULEDATA_H
#define CMSMODULEDATA_H

#include "icmsdata.h"

class CmsModuleData : public ICmsData
{
public:
    CmsModuleData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsModuleData();

    virtual bool fromByteArray(const QByteArray &arr);

    virtual QByteArray toByteArray() const;

    CmsModuleID cmsModuleId() const;

    CmsLabelID cmsLabelId() const;

protected:
    virtual bool decodePacket(const QByteArray &arr) = 0;

    virtual QByteArray encodePacket() const = 0;

protected:
    CmsModuleID         identifier = CMS_ModuleNull;
    CmsLabelID          label = CMS_LabelNull;
    CmsProbeState  probeState = CMS_ProbeStateNormal;
};

#endif // CMSMODULEDATA_H
