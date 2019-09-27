#ifndef CMSIBPMODULEDATA_H
#define CMSIBPMODULEDATA_H

#include "cmsmoduledata.h"

struct IbpWavePoint
{
    bool    rWave = false;
    qint16 ibpWave = 0;        // 1mmHg = 100
};

struct IbpDataField
{
    bool    sensorOff = false;
    bool    catheterOff = false;
    quint8  zeroStatus = 0;
    quint8  calibrationStatus = 0;
    bool    hasRawWave = false;
    qint16  sys = 0;            // 1mmHg = 100
    qint16  dia = 0;            // 1mmHg = 100
    qint16  mean = 0;           // 1mmHg = 100
    quint16 pulseRate = 0;
    qint16  ppv = 0;
    qint16  spv = 0;

    QList<IbpWavePoint> waves;
};

class CmsIbpModuleData : public CmsModuleData
{
public:
    CmsIbpModuleData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsIbpModuleData();

    void setData(IbpDataField data);

    IbpDataField data() const;

protected:
    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

    QByteArray encodeWaves() const;

protected:
    IbpDataField dataField;
    static const int paramSectionSize = 24;
};

Q_DECLARE_METATYPE(IbpDataField)

#endif // CMSIBPMODULEDATA_H
