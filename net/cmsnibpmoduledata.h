#ifndef CMSNIBPMODULEDATA_H
#define CMSNIBPMODULEDATA_H

#include "cmsmoduledata.h"

typedef enum
{
    CMS_NibpManual = 0,                // 手动模式
    CMS_NibpPeriod = 1,                // 周期模式
    CMS_NibpStat = 2,                  // STAT连续测量模式
    CMS_NibpHardwareOverPressure = 3,  // 硬件过压调试模式
    CMS_NibpStaticPressure = 4,        // 静压调试模式
    CMS_NibpLeakDetection = 5,         // 漏气检测模式
    CMS_NibpVenipuncture = 8,          // 静脉穿刺模式
    CMS_NibpSelfcheck = 9,             // 复位自检模式
    CMS_NibpSoftwareOverPressure = 10, // 软件过压测试
    CMS_NibpDebug = 11,                // 调试模式
    CMS_NibpAuxiliaryCycle = 12        // 辅助周期模式
} CmsNibpWorkMode;

typedef enum
{
    CMS_NibpNormal = 0x00,                      // 正常
    CMS_NibpSelfTestFailed = 0x02,              // 自检失败
    CMS_NibpLooseCuff = 0x06,                   // 袖带松
    CMS_NibpAirLeakage = 0x07,                  // 漏气
    CMS_NibpAirPressureError  = 0x08,           // 气压错误
    CMS_NibpWeakSignal = 0x09,                  // 微弱信号
    CMS_NibpRangeExceeded = 0x0a,               // 超出范围
    CMS_NibpExcessiveMotion = 0x0b,             // 过多的运动
    CMS_NibpOverPressureDetected = 0x0c ,       // 检测导过压
    CMS_NibpSignalSaturated = 0x0d,             // 信号饱和
    CMS_NibpAirSystemLeak = 0x0e,               // 气路漏气
    CMS_NibpSystemFailure = 0x0f,               // 系统失败
    CMS_NibpTimeOut = 0x13,                     // 超时
    CMS_NibpCuffTypeError = 0x14,               // 袖带类型错误
    CMS_NibpZeroCaclFailed = 0x15,              // 压力校零失败
    CMS_NibpPressureCalcFaild = 0x16,           // 压力校准失败
    CMS_NibpHardZeroCaclFailed  = 0x17,         // 硬件过压校零失败
    CMS_NibpHardPressureCaclFailed = 0x18,      // 硬件过压校准失败
} CmsNibpErrorID;

struct NibpWavePoint
{
    quint16 wave; // mmHg
};
struct NibpDataField
{
    CmsNibpErrorID     errorCode = CMS_NibpNormal;
    CmsNibpWorkMode    workMode = CMS_NibpManual;
    bool                    working = false;
    bool                    hasResult = false;
    bool                    newResult = false;
    bool                    hasError = false;
    quint16                 sys = InvalidData::nibp;
    quint16                 dia = InvalidData::nibp;
    quint16                 map = InvalidData::nibp;
    quint16                 pr = InvalidData::nibp;

    QList<NibpWavePoint>    waves;
};

class CmsNibpModuleData : public CmsModuleData
{
public:
    CmsNibpModuleData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsNibpModuleData();

    void setData(NibpDataField dataField);

    NibpDataField data() const;

protected:
    virtual bool decodePacket(const QByteArray &arr);

    virtual QByteArray encodePacket() const;

    QByteArray encodeWaves() const;

protected:
    NibpDataField dataField;
    static const int paramSectionSize = 21;
};

Q_DECLARE_METATYPE(NibpDataField)

#endif // CMSNIBPMODULEDATA_H
