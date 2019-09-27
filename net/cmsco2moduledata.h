#ifndef CMSCO2MODULEDATA_H
#define CMSCO2MODULEDATA_H

#include "cmsmoduledata.h"

struct Co2WavePoint
{
    unsigned short co2Wave = 0;
};

struct Co2DataField
{
    bool    working = false;
    bool    etValid = false;
    bool    fiValid = false;
    bool    awrrValid = false;
    quint16 etValue = 0;            // % 乘以100
    quint16 fiValue = 0;            // % 乘以100
    quint16 awrrValue = 0;

    QList<Co2WavePoint> waves;
};

class CmsCo2ModuleData : public CmsModuleData
{
public:
    CmsCo2ModuleData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsCo2ModuleData();

    void setData(Co2DataField data);

    Co2DataField data() const;

protected:
    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

    QByteArray encodeWaves() const;

protected:
    Co2DataField dataField;
    static const int paramSectionSize = 17;
};

Q_DECLARE_METATYPE(Co2DataField)

#endif // CMSCO2MODULEDATA_H
