#ifndef CMSECGCONFIGDATA_H
#define CMSECGCONFIGDATA_H

#include "cmsconfigdata.h"
#include "cmsecgmoduledata.h"

typedef enum
{
    CMS_EcgModeMonitor = 0,     // 监护模式
    CMS_EcgModeOperation = 1,   // 手术模式
    CMS_EcgModeDiangose = 2,    // 诊断模式
    CMS_EcgModeSt = 3,          // ST模式
    CMS_EcgModeUser = 4,        // 用户模式
} CmsEcgWorkMode;

//心律失常代码定义
typedef enum tagNPS_ARR_CODE
{
    CMS_ArrNormal            = 0,              // 正常
    CMS_ArrAsy               = 1,              // 停搏
    CMS_ArrVf                = 2,              // 室颤/室速
    CMS_ArrRun               = 3,              // 连发室早多
    CMS_ArrCpt               = 4,              // 室早成对
    CMS_ArrBgm               = 5,              // 室早二联律
    CMS_ArrTgm               = 6,              // 室早三联律
    CMS_ArrRot               = 7,              // R on T
    CMS_ArrVpb               = 8,              // 单个室早
    CMS_ArrTac               = 9,              // 心动过速
    CMS_ArrBrd               = 10,             // 心动过缓
    CMS_ArrMts               = 11,             // 漏搏
    CMS_ArrSth               = 12,             // 房颤
    CMS_ArrStl               = 13,             // 房颤停止
    CMS_ArrPnc               = 14,             // 起搏器未俘获
    CMS_ArrPnp               = 15,             // 起搏器未起搏
    CMS_ArrApb               = 16,             // 房性早博（保留，未实现）
    CMS_ArrApbaf             = 17,             // 房性早博多发（保留，未实现）
    CMS_ArrNos               = 18,             // 心电波有干扰
    CMS_ArrRes19             = 19,             // 保留19
    CMS_ArrArrlRN            = 20,             // 心律失常自学习
    CMS_ArrArroFF            = 21,             // ARR关闭
    CMS_ArrRes22             = 22,             // 保留22
    CMS_ArrRes23             = 23,             // 保留23
    CMS_ArrRes24             = 24,             // 保留24
    CMS_ArrRes25             = 25,             // 保留25
    CMS_ArrRes26             = 26,             // 保留26
    CMS_ArrRes27             = 27,             // 保留27
    CMS_ArrRes28             = 28,             // 保留28
    CMS_ArrRes29             = 29,             // 保留29
    CMS_ArrVt                = 30,             // 室速
    CMS_ArrPvch              = 31,             // 频发 PVC (需主机下发判定限)
    CMS_ArrNsvt              = 32,             // 非持续性室速
    CMS_ArrEtac              = 33,             // 极度心动过速
    CMS_ArrEbrd              = 34,             // 极度心动过缓
    CMS_ArrPause             = 35,             // 心跳暂停
    CMS_ArrIrhr              = 36,             // 不规则节律
    CMS_ArrVbrd              = 37,             // 室性心动过缓
    CMS_ArrVrhy              = 38,             // 室性节律
    CMS_ArrIrhre             = 39,             // 室性节律
    CMS_ArrPauseH            = 40,             // Pause/min 过高
    CMS_ArrMax               = CMS_ArrPauseH, // 心律失常代码总数
} CmsArrCode;

struct CmsArrAlarmConfig
{
    bool            sw = false;
    bool            support = false;
    CmsAlarmLevel   level = CMS_AlarmTipLevel;
};

struct CmsStAlarmConfig
{
    bool            sw = false;
    CmsAlarmLevel   level = CMS_AlarmMidLevel;

    quint16         limit[CMS_EcgLeadMax][CmsAlarmConfig<>::max_index];
};

struct EcgConfigField : public BaseConfigField
{
    enum
    {
        OP_ProbeInfo = 0x01,
        OP_ArrConfig = 0x02,
        OP_StConfig = 0x04,
    };

    CmsEcgWorkMode     workMode = CMS_EcgModeMonitor;      // 工作模式
    CmsEcgLeadType     leadType;                           // 心电导联类型,几导模式
    bool                    arrSwitch = false;                  // 心律失常分析开关
    bool                    stSwitch = false;                   // st失常分析开关
    bool                    paceSwitch = false;                 // pace开关
    CmsECGLeadIndex         monitorLead = CMS_EcgLeadII;   // 重点监护导联
    CmsAlarmConfig<>        hrAlarmSetting;

    // arr
    CmsArrAlarmConfig       arrAlarmConfig[40];
    quint8                  pvcsLimit;                          // PVCs报警限
    quint16                 stRiseLimit;                        // ST抬高限
    quint16                 stDepressedLimit;                   // ST压低限

    // st
    CmsStAlarmConfig        stAlarmConfig;
};

class CmsEcgConfigData : public CmsConfigData
{
public:
    CmsEcgConfigData(CmsModuleID identifier, CmsLabelID label);

    virtual ~CmsEcgConfigData();

    void setConfig(EcgConfigField config);

    EcgConfigField config() const;

protected:
    virtual bool decodePacket(const QByteArray &arr);

    void decodeArrSection(const QByteArray &rawData);

    void decodeStSection(const QByteArray &rawData);

    virtual QByteArray encodePacket() const;

    QByteArray encodeArrSection() const;

    QByteArray encodeStSection() const;

protected:
    EcgConfigField configField;
    static const int    baseSectionSize = 16;
    static const int    arrSectionSize = 45;
    static const int    stSectionSize = 53;
};

Q_DECLARE_METATYPE(EcgConfigField)

#endif // CMSECGCONFIGDATA_H
