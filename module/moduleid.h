#ifndef MODULEID
#define MODULEID

#include <QObject>

#define PHYSICAL_ECG            0xe8
#define PHYSICAL_RESP           0xe1
#define PHYSICAL_SPO2_BLT       0xf3
//#define PHYSICAL_SPO2_MASIMO    0xe8
//#define PHYSICAL_SPO2_NELLCOR   0xe8
#define PHYSICAL_IBP            0xf9
#define PHYSICAL_NIBP           0xf4
#define PHYSICAL_TEMP           0xe7
#define PHYSICAL_CO             0xe0
#define PHYSICAL_CO2            0xd8
#define PHYSICAL_DM             0xee
#define PHYSICAL_IBOARD         0x30

#define PHYSICAL_SUB_PR         0xf6
#define PHYSICAL_SUB_TIMER      0xf7

typedef enum
{
    MODULE_ID_NULL,
    MODULE_ID_ECG,
    MODULE_ID_SPO2,
    MODULE_ID_IBP,
    MODULE_ID_NIBP,
    MODULE_ID_TEMP,
    MODULE_ID_CO2,
    MODULE_ID_RESP,
    MODULE_ID_CO,
    MODULE_ID_BIS,
    MODULE_ID_AA,
    MODULE_ID_N2O,
    MODULE_ID_O2,
    MODULE_ID_RM,
    MODULE_ID_DM,

    //虚拟模块
    MODULE_ID_PR,
    MODULE_ID_CPP,
    MODULE_ID_TIMER,
    MODULE_ID_SYSTEM,

    // 接口板
    MODULE_ID_IBOARD,

    MODULE_ID_MAX,
} ModuleID;

typedef enum
{
    LABEL_NULL = 0,
    LABEL_ECG = 1,
    LABEL_SPO2 = 2,
    LABEL_NIBP = 3,
    LABEL_RESP = 4,
    LABEL_CO2 = 5,
    LABEL_ICG = 6,
    LABEL_BIS = 7,
    LABEL_AA = 8,
    LABEL_O2 = 9,
    LABEL_N2O = 10,
    LABEL_PR = 11,
    LABEL_CO = 14,
    LABEL_RM = 16,
    LABEL_IBP_START = 20,
    LABEL_IBP_P1 = LABEL_IBP_START,
    LABEL_IBP_P2 = 21,
    LABEL_IBP_ART = 22,
    LABEL_IBP_CVP = 23,
    LABEL_IBP_PA = 24,
    LABEL_IBP_RAP = 25,
    LABEL_IBP_LAP = 26,
    LABEL_IBP_ICP = 27,
    LABEL_IBP_PAWP = 28,
    LABEL_IBP_UAP = 29,
    LABEL_IBP_BAP = 30,
    LABEL_IBP_FAP = 31,
    LABEL_IBP_UVP = 32,
    LABEL_IBP_LV = 33,
    LABEL_IBP_AO = 34,
    LABEL_IBP_END = LABEL_IBP_AO,
    LABEL_IPB_RESERVED = 35,
    LABEL_TOF = 36,
    LABEL_CPP = 37,
    LABEL_TEMP_START = 50,
    LABEL_TEMP_A = 50,
    LABEL_TEMP_B = 51,
    LABEL_TEMP_C = 52,
    LABEL_TEMP_D = 53,
    LABEL_TEMP_E = 54,
    LABEL_TEMP_END = LABEL_TEMP_E,
    LABEL_TEMP_RESERVED1 = 58,
    LABEL_TEMP_RESERVED2 = 59,
    LABEL_DM = 70,

    LABEL_ST,
    LABEL_QTC,
    LABEL_ARR,
    LABEL_RANDOM_IBP,
    LABEL_OVERLAP_IBP,
    LABEL_RANDOM_TEMP,

    LABEL_TIMER_START = 200,
    LABEL_TIMER1 = LABEL_TIMER_START,
    LABEL_TIMER2,
    LABEL_TIMER3,
    LABEL_TIMER4,  // 203
    LABEL_NIBP_LIST,
    LABEL_IBOARD,
    LABEL_SYSTEM,
    LABEL_RESP_S,

    LABEL_CLOSE,

    LABEL_MAX
} LabelID;


typedef enum
{
    WAVE_NULL = 0,
    WAVE_ECG_START = 1,
    WAVE_ECG_I = WAVE_ECG_START,
    WAVE_ECG_II,
    WAVE_ECG_III,
    WAVE_ECG_AVR,
    WAVE_ECG_AVL,
    WAVE_ECG_AVF,
    WAVE_ECG_V1,
    WAVE_ECG_V2,
    WAVE_ECG_V3,
    WAVE_ECG_V4,
    WAVE_ECG_V5,
    WAVE_ECG_V6,
    WAVE_ECG_END = WAVE_ECG_V6,
    WAVE_SPO2,
    WAVE_RESP,
    WAVE_CO2,
    WAVE_ICG,
    WAVE_BIS,
    WAVE_AA,
    WAVE_O2,
    WAVE_N2O,
    WAVE_PR,
    WAVE_CO,
    WAVE_RM,
    WAVE_IBP_START = 40,
    WAVE_IBP_P1 = WAVE_IBP_START,
    WAVE_IBP_P2,
    WAVE_IBP_ART,
    WAVE_IBP_CVP,
    WAVE_IBP_PA,
    WAVE_IBP_RAP,
    WAVE_IBP_LAP,
    WAVE_IBP_ICP,
    WAVE_IBP_PAWP,
    WAVE_IBP_UAP,
    WAVE_IBP_BAP,
    WAVE_IBP_FAP,
    WAVE_IBP_UVP,
    WAVE_IBP_LV,
    WAVE_IBP_AO,
    WAVE_IBP_END = WAVE_IBP_AO,
    WAVE_IPB_RESERVED = 55,

    WAVE_ECG_ST_SEGMENT,
    WAVE_ECG_ST_GRAPHIC,
    WAVE_RANDOM_IBP,
    WAVE_OVERLAP_IBP,
    WAVE_CLOSE,
    WAVE_AUTO,

    WAVE_MAX
} WaveID;


enum SubIbpParamDefine
{
    Sub_IbpNull = 0,
    Sub_IbpSys = Sub_IbpNull,
    Sub_IbpDia,
    Sub_IbpMean,
    Sub_IbpPr,
    Sub_IbpCpp,
    Sub_IbpMax
};


enum SubNibpParamDefine
{
    Sub_NibpNull = 0,
    Sub_NibpSys = Sub_NibpNull,
    Sub_NibpDia,
    Sub_NibpMap,
    Sub_NibpSdp,
    Sub_NibpPr,
    Sub_NibpTime,
    Sub_NibpMax
};

enum SubEcgParamDefine
{
    Sub_EcgNull = 0,
    Sub_Hr = Sub_EcgNull,//ECG
    Sub_Qtc,
    Sub_QtcDiff,
    Sub_StI,
    Sub_StII,
    Sub_StIII,
    Sub_StAVR,
    Sub_StAVL,
    Sub_StAVF,
    Sub_StV1,
    Sub_StV2,
    Sub_StV3,
    Sub_StV4,
    Sub_StV5,
    Sub_StV6,

    SubARR_ASY,            //停搏            //致命性
    SubARR_VF,             //室颤            //致命性
    SubARR_VBRD,           //室性心动过缓     //致命性
    SubARR_ETAC,           //极度心动过速     //致命性
    SubARR_EBRD,           //极度心动过缓     //致命性
    SubARR_VT,             //室速            //致命性
    SubARR_RUN,            //连发室早多
    SubARR_CPT,            //室早成对
    SubARR_BGM,            //室早二连律
    SubARR_TGM,            //室早三联律
    SubARR_ROT,            //R ON  T
    SubARR_VPB,            //单个室早
    SubARR_TAC,            //心动过速
    SubARR_BRD,            //心动过缓
    SubARR_MIS,            //漏博
    SubARR_AFIB,           //房颤
    SubARR_AFIBEND,        //房颤停止
    SubARR_PNP,            //起搏器为起搏
    SubARR_PNC,            //起搏器未俘获
    SubARR_NOS,            //心电波有干扰
    SubARR_PVCH,           //频发
    SubARR_NSVT,           //非持续性室速
    SubARR_VRHY,           //室性节律
    SubARR_PAUSE,          //心动暂停
    SubARR_IRHR,           //不规则节律
    SubARR_PAUSEH,         //pause/min high
    SubARR_IRHRE,          //不规则节律停止
    Sub_EcgMax
};



enum SubSpo2ParamDefine
{
    Sub_Spo2Null = 0,
    Sub_Spo2 = Sub_Spo2Null,//SPO2
    Sub_Spo2Diff,
    Sub_Spo2Pi,
    Sub_Spo2Max
};

enum SubTempParamDefine
{
    Sub_TempNull = 0,

    // channel 1
    Sub_TempT01 = 1,
    Sub_TempT02,

    // channel 2
    Sub_TempT03,
    Sub_TempT04,

    // channel 3
    Sub_TempT05,
    Sub_TempT06,

    // channel 4
    Sub_TempT07,
    Sub_TempT08,

    // channel 5
    Sub_TempT09,
    Sub_TempT10,

    // these can be select to all channels
    Sub_TempAliasStart,                     // 体温别名开始
    Sub_TempSkin = Sub_TempAliasStart,      // 体表 skin
    Sub_TempCore,                           // 核心 core
    Sub_TempAxil,                           // 腋下 axilla
    Sub_TempNaso,                           // 鼻咽 nasopharynx
    Sub_TempEso,                            // 食道 esophagus
    Sub_TempRect,                           // 直肠 rectum
    Sub_TempAmb,                            // 环境 ambient
    Sub_TempAirw,                           // 气道 airway
    Sub_TempVesic,                          // 膀胱 vesical
    Sub_TempBlood,                          // 血液 blood
    Sub_TempMyo,                            // 心肌 myocardium
    Sub_TempTymp,                           // 鼓膜 tympanic
    Sub_TempCereb,                          // 大脑 cerebrum
    Sub_TempAliasEnd = Sub_TempCereb,       // 体温别名结束

    Sub_TempTd,                             // temp diff

    Sub_TempMax
};

enum SubRespParamDefine
{
    Sub_RespNull = 0,
    Sub_Resp = Sub_RespNull,
    Sub_RespSource,
    Sub_RespMax
};

enum SubPrParamDefine
{
    Sub_PrNull = 0,
    Sub_Pr = Sub_PrNull,
    Sub_PrSource,
    Sub_PrMax
};

enum SubCOParamDefine
{
    Sub_CONull = 0,
    Sub_COCO = Sub_CONull,
    Sub_COCI,
    Sub_COTI,
    Sub_COTB,
    Sub_COMax
};

enum SubCO2ParamDefine
{
    Sub_CO2Null = 0,
    Sub_CO2Et = Sub_CO2Null,
    Sub_CO2Fi,
    Sub_CO2awRR,
    Sub_CO2Max
};

enum SubParamId
{

};
Q_DECLARE_METATYPE(WaveID)
Q_DECLARE_METATYPE(LabelID)

#endif // MODULEID

