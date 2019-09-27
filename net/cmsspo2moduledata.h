#ifndef CMSSPO2MODULEDATA_H
#define CMSSPO2MODULEDATA_H

#include "cmsmoduledata.h"

struct Spo2WavePoint
{
    bool    rwave = false;
    quint8  bloodStrength = 0;
    quint8  spo2Wave = 0;
    quint8  respWave = 0;
};

struct Spo2DataField
{
    bool sensorOff = false;
    bool cableOff = false;
    bool weakPulse = false;
    bool searching = false;
    bool interference = false;
    bool hasRawWave = false;
    struct
    {
        bool    support = false;
        bool    enable = false;
        bool    status = false;
        quint8  respRate = 0;
    } resp;

    quint8  sqi = 100;
    quint8  spo2= 255;
    quint16 pi = 0;                     // 扩大了10000倍之后的数据
    quint16 pulseRate = 0;

    struct
    {
        bool    support = false;
        quint16 spco = 0;
        quint16 spmet= 0;
        quint8  sphb= 0;
        quint16 spoc = 0;
        quint8  destIndex = 0;          // deast index
        quint16 deltaPi = 0;
        quint8  pvi = 0;
    } rainbow;

    QList<Spo2WavePoint> waves;
    QList<Spo2WavePoint> rawWaves;
};

class CmsSpo2ModuleData : public CmsModuleData
{
public:
    CmsSpo2ModuleData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsSpo2ModuleData();

    void setData(Spo2DataField data);

    Spo2DataField data() const;

protected:
    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

    QByteArray encodeWaves() const;

    QByteArray encodeRawWaves() const;

protected:
    Spo2DataField dataField;
    static const int paramSectionSize = 33;
};

Q_DECLARE_METATYPE(Spo2DataField)

#endif // CMSSPO2MODULEDATA_H
