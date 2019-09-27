#ifndef CMSTEMPMODULEDATA_H
#define CMSTEMPMODULEDATA_H

#include "cmsmoduledata.h"

struct TempDataField
{
    bool isDualTemp = false;            // 是否是双通道

    struct
    {
        uint8_t     subid;              // 子通道的名字
        uint8_t     measureStatus;      // 测量状态
        uint16_t    data;               // 子通道的数据, 放大100倍
    } temp[2];
};

class CmsTempModuleData : public CmsModuleData
{
public:
    CmsTempModuleData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsTempModuleData();

    void setData(TempDataField data);

    TempDataField data() const;

protected:
    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

protected:
    CmsProbeState  probeState2 = CMS_ProbeStateNormal;
    TempDataField  dataField;
    static const int    paramSectionSize = 16;
};

Q_DECLARE_METATYPE(TempDataField)

#endif // CMSTEMPMODULEDATA_H
