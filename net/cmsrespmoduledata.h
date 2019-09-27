#ifndef CMSRESPMODULEDATA_H
#define CMSRESPMODULEDATA_H

#include "cmsmoduledata.h"

typedef enum
{
} RespCalcLead;

typedef enum
{
    RESP_Normal,
    RESP_Inspiration,
    RESP_Expiration,
} RespPhase;

struct RespWavePoint
{
    RespPhase   phase;
    qint16      respWave;
};

struct RespDataField
{
    bool    sensorOff = false;
    bool    notUpdated = false;
    bool    apnea = false;
    bool    cardiacInterference = false;
    quint8  calcLead = 0;    // 0 -> RA_LL, 1 -> RA_LA
    quint8  respRate = 255;

    QList<RespWavePoint> waves;
};

class CmsRespModuleData : public CmsModuleData
{
public:
    CmsRespModuleData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsRespModuleData();

    void setData(RespDataField data);

    RespDataField data() const;

protected:
    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

    QByteArray encodeWaves() const;

protected:
    RespDataField dataField;
    static const int paramSectionSize = 15;
};

Q_DECLARE_METATYPE(RespDataField)


#endif // CMSRESPMODULEDATA_H
