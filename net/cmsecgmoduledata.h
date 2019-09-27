#ifndef CMSECGMODULEDATA_H
#define CMSECGMODULEDATA_H

#include "cmsmoduledata.h"

Q_DECLARE_METATYPE(bool *)

typedef enum
{
    CMS_EcgType3,
    CMS_EcgType5,
    CMS_EcgType6,
    CMS_EcgType12,
} CmsEcgLeadType;


typedef enum
{
    CMS_EcgLeadStart = 0,
    CMS_EcgLeadI = CMS_EcgLeadStart,
    CMS_EcgLeadII = 1,
    CMS_EcgLeadIII = 2,
    CMS_EcgLeadAvr = 3,
    CMS_EcgLeadAvl = 4,
    CMS_EcgLeadAvf = 5,

    /*
     * 3导时为III
     * 5导时为V_
     * 6导时为Va
     */
    CMS_EcgLeadV1 = 6,

    /*
     * 6导时为Vb波形.
     */
    CMS_EcgLeadV2 = 7,
    CMS_EcgLeadV3 = 8,
    CMS_EcgLeadV4 = 9,
    CMS_EcgLeadV5 = 10,
    CMS_EcgLeadV6 = 11,
    CMS_EcgLeadMax,
} CmsECGLeadIndex;

typedef enum
{
    CMS_EcgElecRa = 0,
    CMS_EcgElecRl = 1,
    CMS_EcgElecLa = 2,
    CMS_EcgElecLl = 3,
    CMS_EcgElecC1 = 4,         // 5导时传V_,6导时传Va
    CMS_EcgElecC2 = 5,         // 6导时传Vb
    CMS_EcgElecC3 = 6,
    CMS_EcgElecC4 = 7,
    CMS_EcgElecC5 = 8,
    CMS_EcgElecC6 = 9,
    CMS_EcgElecMax = 10,
} CmsEcgElectrode;

struct EcgWavePoint
{
    bool    rwave = false;
    bool    pace = false;

    /*
     * 数据是本机的数据,打包时格式化成协议.解包时,要转回来
     */
    int     wave[8] = {0}; // I, II, V1, V2, V3, V4, V5, V6的波形
};

struct EcgDataField
{
    CmsEcgLeadType          leadType;
    CmsECGLeadIndex         calcLeadIndex;
    bool                    electrodeOff[CMS_EcgElecMax] = {false};
    bool                    polarization[CMS_EcgLeadMax] = {false};
    bool                    hasRawWaves = false;
    quint16                 hr;
    QList<EcgWavePoint>     waves;
    QList<EcgWavePoint>     rawWaves;
};

class CmsEcgModuleData : public CmsModuleData
{
public:
    CmsEcgModuleData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsEcgModuleData();

    void setData(EcgDataField data);

    EcgDataField data() const;

protected:
    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

    QByteArray encodeWaves() const;
    QByteArray encodeRawWaves() const;

protected:
    EcgDataField        dataField;
    static const int    paramSectionSize = 19;
};

Q_DECLARE_METATYPE(EcgDataField)

#endif // CMSECGMODULEDATA_H
