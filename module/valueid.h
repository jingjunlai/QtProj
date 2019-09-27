#ifndef VALUEID
#define VALUEID

#include <QObject>

#define NIBP_SQUENCE_CONTINUOUS 0xFFFF
#define ALARM_PAUSE_PERMANTENT  0xFF
#define NIBP_SAFE_PERIOD 40
#define SEQUENCE_FOREVER  0xFFFF

namespace Gps
{
typedef enum
{
    Facade_Normal = 0,
    Facade_Ani,
} WaveFacade;


enum DateType
{
    Date_Year = 0,
    Date_Month,
    Date_Day,
    Date_Max
};

typedef enum
{
    SL_Standard,
    SL_BigFont,
    SL_EcgScreen,
    SL_DynamicTrend,
    SL_OtherBedView,
    SL_OxyCRG,
    SL_Max
} ScreenLayoutType;

typedef enum
{
    SS_10Inches_1,          // 10寸版本1, 211*158mm
    SS_12Inches_1,          // 12寸版本1, 246*184mm
    SS_Max
} ScreenPhysicalSize;

typedef enum
{
    BuildinTemp_Single,
    BuildinTemp_Dual,
} BuildinTempType;

enum PatientType
{
    Patient_Adult = 0,
    Patient_Ped,
    Patient_Neo,
    Patient_End = Patient_Neo,
    Patient_Max
};

enum Sex
{
    Male = 0,
    Female
};

enum PaceType
{
    NoPace,
    Pace,
    AutoPace
};

enum BloodType
{
    A = 0,
    B,
    AB,
    O,
    A_,
    B_,
    AB_,
    O_
};

enum
{
    False = 0,
    No = False,
    Off = False,
    Disable = False,

    True = 1,
    Yes = True,
    On = True,
    Enable = True
};

enum AlarmLevel
{
    Alarm_TipLevel = 0,
    Alarm_LowLevel,
    Alarm_MidLevel,
    Alarm_HighLevel,
    Alarm_LockOff, //做为栓锁报警关闭的参数值
};

enum AlarmType
{
    Alarm_Tec = 0,
    Alarm_Phy,
};

typedef enum
{
    Speed_6_25 = 0,
    Speed_12_5,
    Speed_25,
    Speed_50,
} WaveSpeed;

typedef enum
{
    Gain_Auto = 0,
    Gain_0_125,
    Gain_0_25,
    Gain_0_5,
    Gain_1,
    Gain_2,
    Gain_4,
} WaveGain;

typedef enum    //身高单位
{
    Unit_cm = 0,
    Unit_inch,
}UnitHeight;

typedef enum   //体重单位
{
    Unit_kg = 0,
    Unit_lb,
}UnitWeight;

typedef enum    //幅度单位
{
    Unit_mV = 0,
} UnitAmplitude;

typedef enum     //浓度单位
{
    Unit_g_per_dl = 0,
    Unit_mmol_per_L,
    Unit_g_per_L,
}UnitConcentration;

typedef enum      //压强单位
{
    Unit_mmHg = 0,
    Unit_kPa,
    Unit_cmH2O,
    Unit_Percent,
}UnitPressure;

typedef enum     //体温单位
{
    Unit_Centigrade = 0,
    Unit_Fahrenheit,
}UnitTemp;

typedef enum ecgLead
{
    ECG_LEAD_I = 0,
    ECG_LEAD_II,
    ECG_LEAD_III,
    ECG_LEAD_AVR,
    ECG_LEAD_AVL,
    ECG_LEAD_AVF,
    ECG_LEAD_V_1,
    ECG_LEAD_V_2,
    ECG_LEAD_V_3,
    ECG_LEAD_V_4,
    ECG_LEAD_V_5,
    ECG_LEAD_V_6,
    ECG_LEAD_MAX
} ECG_LEAD;
#define ECG_LEAD_VA     ECG_LEAD_V_1
#define ECG_LEAD_VB     ECG_LEAD_V_2

typedef enum            //起搏器
{
    ECG_Pace_UnDefine = 0,      //未指定
    ECG_Pace_Yes,               //是
    ECG_Pace_NO,                //否
    ECG_PaceMax
} EcgPaceType;

typedef enum ecgleadtype
{
    ECG_LeadType_3 = 0,
    ECG_LeadType_5,
    ECG_LeadType_6,
    ECG_LeadType_12,
    ECG_LeadType_Auto,
    ECG_LeadTypeMax
} EcgLeadType;

typedef enum ecgElectrode
{
    EcgElecRa = 0,
    EcgElecRl = 1,
    EcgElecLa = 2,
    EcgElecLl = 3,
    EcgElecC1 = 4,         // 5导时传V_,6导时传Va
    EcgElecC2 = 5,         // 6导时传Vb
    EcgElecC3 = 6,
    EcgElecC4 = 7,
    EcgElecC5 = 8,
    EcgElecC6 = 9,
    EcgElecMax = 10,
} EcgElectrode;

typedef enum hralarmsource      //HR报警来源
{
    SourceAuto = 0,
    SourceHR,
    SourcePR,
    SourceHRAndPR,
    SourceMax,
} HRAlarmSource;

typedef enum ecgmode            //3，5，6监护，手术，诊断，用户，ST 12导：诊断，ST
{
    ECG_ModeMonitor = 0,        //监护
    ECG_ModeOperation,          //手术
    ECG_ModeDiagnose,           //诊断
    ECG_ModeST,                 //ST
    ECG_ModeUser,               //用户
    ECG_ModeMax
} EcgMode;

typedef enum volume       //音量
{
    Volume_0 = 0,
    Volume_1,        //1
    Volume_2,
    Volume_3,
    Volume_4,
    Volume_5,
    Volume_6,
    Volume_7,
    Volume_8,
    Volume_9,
    Volume_10,
    Volume_Max
} Volume;

typedef enum brightness       //音量
{
    Brightness_Auto = 0,
    Brightness_1,        //1
    Brightness_2,
    Brightness_3,
    Brightness_4,
    Brightness_5,
    Brightness_6,
    Brightness_7,
    Brightness_8,
    Brightness_9,
    Brightness_10,
    Brightness_Max
} Brightness;

typedef enum
{
    Language_English,
    Language_SimChinese,
    Language_French,
    Language_German,
    Language_Italian,
    Language_Polish,
    Language_Spanish,
    Language_Portuguese,
    Language_Russian,
    Language_Czech,
    Language_Turkish,
    Language_Danish,
    Language_Dutch,
    Language_Finish,
    Language_Hungarian,
    Language_Norwegian,
    Language_Swedish,
    Language_Romanian,
    Language_Serian,
    Language_TraChinese,
    Language_Greek,
} Language;

typedef enum
{
    RetentionTime_1Min,
    RetentionTime_3Min,
    RetentionTime_5Min,
    RetentionTime_Max,
}RetentionTime;

typedef enum
{
    Authorization_NoPassword,
    Authorization_Password,
}Authorization;

typedef enum
{
    AuthorizationType_AlarmSetup,
    AuthorizationType_ConfigSetup,
    AuthorizationType_Max,
}AuthorizationType;

typedef enum
{
    Config_Current,
    Config_Default,
} ConfigType;

typedef enum
{
    IP_Automatically,
    IP_Static,
} IP_Method;

typedef enum            //肌电滤波
{
    ECG_EmgFilter_Weak = 0,         //关闭（弱）
    ECG_EmgFilter_Mild,             //温和45HZ
    ECG_EmgFilter_Strong,           //强25HZ
    ECG_EmgFilterMax
} EcgEMGFilter;

typedef enum            //漂移滤波
{
    ECG_DriftFilter_Weak = 0,      //关闭3.2s   弱
    ECG_DriftFilter_Mild,          //打开1 0.3s   温和
    ECG_DriftFilter_Strong,        //打开2 0.16s  强
    ECG_DriftFilterMax
} EcgDriftFilter;

typedef enum ecgpowerfrequency //ECG工频滤波
{
    ECG_Power_50HZ = 0,
    ECG_Power_60HZ,
    ECG_PowerMax
} EcgPowerFrequency;

typedef enum qtcalculateformula //qt计算公式索引
{
    QTCalculate_Hodges = 0,
    QTCalculate_Bazett,
    QTCalculate_Fridericia,
    QTCalculate_Framingham,
    QTCalculate_Max
} QTCalculateFormula;

typedef enum ecgsignallevel             //信号质量级别
{
    ECG_SigLevel_0 = 0,         //0
    ECG_SigLevel_1,             //    (0,20]
    ECG_SigLevel_2,             //(20, 40]
    ECG_SigLevel_3,             //(40, 60]
    ECG_SigLevel_4,             //(60, 80]
    ECG_SigLevel_5,             //(80. 100]
    ECG_SigLevelMax
} ECGSignalLevel;

typedef enum spo2Sensitivity
{
    SpO2_Sensitivity_Mid = 0,
    SpO2_Sensitivity_High,
    SpO2_Sensitivity_Low,
    SpO2_Sensitivity_Max,
} SpO2Sensitivity;

typedef enum PrSource
{
    PR_Src_Auto = 0,
    PR_Src_SpO2,
    PR_Src_IBP,
    PR_Src_Art,
    PR_Src_Ao,
    PR_Src_PA,
    PR_Src_UAP,
    PR_Src_FAP,
    PR_Src_BAP,
    PR_Src_LV,
    PR_Src_P1,
    PR_Src_P2,
    PR_Src_Max,
} PRSource;

typedef enum  //IBP灵敏度
{
    IBP_SensitivityMid = 0,
    IBP_SensitivityHigh,
    IBP_SensitivityLow,
    IBP_SensitivityMax,
} IbpSensitivity;

typedef enum  //IBP滤波设置
{
    IBP_WaveFilterMighty = 0,
    IBP_WaveFilterMiddle,
    IBP_WaveFilterMild,
    IBP_WaveFilterMax
} IbpWaveFilter;

//typedef enum //IBP单位
//{
//    Ibp_Unit_mmHg = 0,
//    Ibp_Unit_kPa,
//    Ibp_Unit_cmH2O,
//    Ibp_Unit_Max
//} IbpUnit;

typedef enum
{
    CO2SensorType_C5 = 0,
    CO2SensorType_Masimo,
    CO2SensorType_CPT,
}CO2SensorType;

typedef enum //校零气体类型
{
    GasType_OnN2 = 0,
    GasType_OnRoomAir,
}CO2ZeroGasType;

typedef enum //
{
    WorkMode_Work = 0,
    WorkMode_Sleep,
}CO2WorkMode;

typedef enum //平衡气体
{
    Balance_RoomAir = 0,
    Balance_N2O,
    Balance_Helium,
}CO2BalanceGas;

typedef enum //呼末周期
{
    OneBreathPeriod = 0,
    Period_10sec,
    Period_20sec
}CO2EtPeriod;

enum
{
    CO2Scale_10_Per = 0,  //对应百分比的 10.0  mmhg的 80 只是菜单对应，数值换算不对应的
    CO2Scale_8_Per,       //对应百分比的 8.0   mmhg的 60
    CO2Scale_7_Per,       //对应百分比的 7.0   mmhg的 50
    CO2Scale_5_Per,       //对应百分比的 5.0   mmhg的 40
    CO2Scale_3_5_Per,     //对应百分比的 3.5   mmhg的 25
    CO2Scale_2_5_Per,     //对应百分比的 2.5   mmhg的 20
    CO2Scale_2_Per,       //对应百分比的 2.0   mmhg的 15
    CO2Scale_Max,
};

typedef enum //IBP标尺类型
{
    Ibp_RulerAuto = 0,   //自动标尺范围
    Ibp_RulerManual,     //手动标尺范围
    Ibp_RulerMax
} IbpRulerType;

typedef enum respsource   //RR来源
{
    Resp_Source_Auto = 0,
    Resp_Source_Co2,
    Resp_Source_Rm,
    Resp_Source_EcgBoard,
    Resp_Source_SpO2,
    Resp_Source_Max,
} RespSource;

typedef enum respleadtype
{
    RESP_Lead_RA_LL = 0,
    RESP_Lead_RA_LA,
    RESP_Lead_Auto,
    RESP_Lead_Max
} RespLeadType;

typedef enum respapneadelaytime
{
    RESP_ApneaDelay_10 = 10,             //窒息延迟10s
    RESP_ApneaDelay_15 = 15,             //窒息延迟15s
    RESP_ApneaDelay_20 = 20,             //窒息延迟20s
    RESP_ApneaDelay_25 = 25,             //窒息延迟25s
    RESP_ApneaDelay_30 = 30,             //窒息延迟30s
    RESP_ApneaDelay_35 = 35,             //窒息延迟35s
    RESP_ApneaDelay_40 = 40,             //窒息延迟40s
    RESP_ApneaDelayMax,
} RespApneaDelayTime;

typedef enum nibperrcode
{
    Nibp_NoErr,                            //无错误
    Nibp_NoDefine1,                        //未定义
    Nibp_SelfTestFailed,                   //自检错误
    Nibp_NoErr1,                           //无错误
    Nibp_NoDefine4,                        //未定义
    Nibp_NoDefine5,                        //未定义
    Nibp_LooseCuff,                        //袖带松
    Nibp_AirLeakage,                       //漏气
    Nibp_AirPressureErr,                   //气压错误
    Nibp_WeakSignal,                       //微弱信号
    Nibp_RangeExceeded,                    //超出范围
    Nibp_ExcessiveMotion,                  //过多运动
    Nibp_OverPressureSensed,               //检测到过压
    Nibp_SignalSaturated,                  //信号饱和
    Nibp_AirSystemLeak,                    //气路漏气
    Nibp_SystemFailure,                   //系统失败
    Nibp_NoDefine10,                       //未定义
    Nibp_NoDefine11,                       //未定义
    Nibp_NoDefine12,                       //未定义
    Nibp_TimeOut,                          //超时
    Nibp_CuffTypeErr,                      //袖带类型错
    Nibp_ZeroFailed,                       //压力校零失败
    Nibp_CalibrationFailed,                //压力较准失败
    Nibp_HardOverPressZeroFailed,          //硬件过压压力校零失败
    Nibp_HardOverPressCaliFailed,          //硬件过压压力校准失败
    Nibp_Err_MAX
}NIBP_ErrCode;

typedef enum squencephasename
{
    Nibp_SquenceA,
    Nibp_SquenceB,
    Nibp_SquenceC,
    Nibp_SquenceD,
    Nibp_SquenceE,
    Nibp_SquenceMax,
}SquencePhaseName;

typedef enum nibpsquencetotaltime
{
    NIBP_Continous,
    NIBP_Off,
    NIBP_Total24h,
    NIBP_Total12h,
    NIBP_Total8h,
    NIBP_Total4h,
    NIBP_Total3h,
    NIBP_Total2h,
    NIBP_Total1h,
    NIBP_Total30min,
    NIBP_Total15min,
    NIBP_TotalMax,
} NibpSquenceTotalTime;

typedef enum nibpInterTime
{
    NIBP_Manual,
    NIBP_sequence,
    NIBP_8h,
    NIBP_4h,
    NIBP_3h,
    NIBP_2h,
    NIBP_1_5h,
    NIBP_1h,
    NIBP_30min,
    NIBP_20min,
    NIBP_15min,
    NIBP_10min,
    NIBP_5min,
    NIBP_3min,
    NIBP_2_5min,
    NIBP_2min,
    NIBP_1min,
    NIBP_IntervalMax,
} NibpInterTime;

typedef enum nibpstartmode
{
    NIBP_Interval,
    NIBP_Clock,
} NibpStartMode;

typedef enum
{
    CO_InjectionVolume_3CC = 0,
    CO_InjectionVolume_5CC,
    CO_InjectionVolume_10CC,
} CO_InjectionVolume;

typedef enum
{
    CO_TISource_Auto = 0,
    CO_TISource_Manual,
} CO_TISource;

typedef enum
{
    CO_MeasureMode_Continuous = 0,
    CO_MeasureMode_Single,
} CO_MeasureMode;

#define DEFAULT_ALARM_LEVEL         Alarm_MidLevel
enum ARRAlarmIndex      //心律失常报警索引 致命性报警只有高级,且报警不能关闭
{
    ARR_ASY = 0,        //停搏            //致命性
    ARR_VF,             //室颤            //致命性
    ARR_VBRD,           //室性心动过缓     //致命性
    ARR_ETAC,           //极度心动过速     //致命性
    ARR_EBRD,           //极度心动过缓     //致命性
    ARR_VT,             //室速            //致命性
    ARR_RUN,            //连发室早多
    ARR_CPT,            //室早成对
    ARR_BGM,            //室早二连律
    ARR_TGM,            //室早三联律
    ARR_ROT,            //R ON  T
    ARR_VPB,            //单个室早
    ARR_TAC,            //心动过速
    ARR_BRD,            //心动过缓
    ARR_MIS,            //漏博
    ARR_AFIB,           //房颤
    ARR_AFIBEND,        //房颤停止
    ARR_PNP,            //起搏器为起搏
    ARR_PNC,            //起搏器未俘获
    ARR_NOS,            //心电波有干扰
    ARR_PVCH,           //频发
    ARR_NSVT,           //非持续性室速
    ARR_VRHY,           //室性节律
    ARR_PAUSE,          //心动暂停
    ARR_IRHR,           //不规则节律
    ARR_PAUSEH,         //pause/min high
    ARR_IRHRE,          //不规则节律停止
    ARR_MAX
};

typedef enum
{
    Ibp_MeasureAuto = 0,  //自动
    Ibp_MeasureAll,       //全部
    Ibp_MeasureMean,      //平均压
    Ibp_typeMax,
} IbpMeasureType;

typedef enum
{
    Recorder_GaryI = 1,  //记录仪打印灰度1
    Recorder_GaryII,     //记录仪打印灰度2
    Recorder_GaryIII,    //记录仪打印灰度3
    Recorder_GaryMax,
}RecorderGary;

enum DateFormat
{
    DateFormat_yyyy_mm_dd,
    DateFormat_mm_dd_yyyy,
    DateFormat_dd_mm_yyyy,
};

typedef enum
{
    Sound_AlarmHigh,
    Sound_AlarmMid,
    Sound_AlarmLow,
    Sound_NibpEnd,
    Sound_AlarmTip,
    Sound_Beep,
    Sound_Audition,
    Sound_Max,
}SoundId;

typedef unsigned int DisplayColor;
const DisplayColor ColorRed = 0xff0000;                 // 纯红色
const DisplayColor ColorOrangeRed = 0xff4500;           // 橙红色
const DisplayColor ColorOrange = 0xffa500;              // 橙色
const DisplayColor ColorSeaShell = 0xfff5ee;            // 海贝壳
const DisplayColor ColorYellow = 0xffff00;              // 纯黄色
const DisplayColor ColorGreen = 0x00ff00;               // 纯绿色
const DisplayColor ColorGreenYellow = 0xadff27;         // 绿黄色
const DisplayColor ColorSpringGreen = 0x3cb371;         // 春天的绿
const DisplayColor ColorTurquoise = 0x40e0d0;           // 绿宝石
const DisplayColor ColorCyan = 0x00ffff;                // 青色
const DisplayColor ColorSkyBlue = 0x87ceeb;             // 天蓝
const DisplayColor ColorSlateBlue = 0x6a5acd;           // 石蓝色,板岩蓝
const DisplayColor ColorPurple = 0x800080;              // 紫色
const DisplayColor ColorMagenta = 0xff00ff;             // 洋红
const DisplayColor ColorPink = 0xffc0cb;                // 粉红
const DisplayColor ColorWhite = 0xffffff;               // 白色
const DisplayColor ColorDullRed = 0x900000;              // 暗红色

const int INVALID_SHORTCUT_KEY = -1;                    // 无效快捷键,main button group的按钮使用

const QString INVALID_DATA_STRING = "--";               // 无效字符串,参数值无效时使用

const int OTHERBED_MIN_NO = 1;                          // 它床编号最小值
const int OTHERBED_MAX_NO = 66;                         // 它床编号最大值


// 除颤同步类型
typedef enum
{
   Defibrillate_Off,
   Defibrillate_50ms,
   Defibrillate_100ms
} DefibrillateSyncType;

// 模拟输出开关
typedef enum
{
    Analog_OutputOff,
    Analog_OutputEcg,
    Analog_OutputIbp,
} AnalogOutputType;

// 模拟输出通道
typedef enum
{
    Analog_ChannelOff,      // 模拟输出通道关
    Analog_ChannelEcg,
    Analog_ChannelIbp1,     // NITS的IBP0模拟输出
    Analog_ChannelIbp2,
    Analog_ChannelIbp3,     // COIBP的IBP0模拟输出
    Analog_ChannelIbp4,
} AnalogOutputChannel;

// 计时器类型
typedef enum
{
    Timer_Type_Normal = 0,          // 基本型
    Timer_Type_Cycled,              // 循环型
    Timer_Type_Unlimited,           // 无限型
} TimerType;

// 计时方向
typedef enum
{
    Timer_Direction_Up = 0,         // 正计时
    Timer_Direction_Down,           // 倒计时
} TimerDirection;

// 运行时间
typedef enum
{
    Timer_RunTimeValue = 5,         // 默认是5分钟
} TimerRunTime;

// 状态栏
typedef enum
{
    /***status***/
    StatusBar_AlarmPauseIcon,
    StatusBar_VolumeIcon,
    StatusBar_UsbIcon,
    StatusBar_NetworkIcon,
    StatusBar_BatteryIcon,
    StatusBar_Voice,
    StatusBar_Screenshot,
}StatusBarIcon;

enum ECGReportAmplitude             //波形幅度
{
    ECGReportAmp_20 = 0,            //20mm/mv
    ECGReportAmp_10,                //10mm/mv
    ECGReportAmp_5,                 //5mmm/mv
    ECGReportAmp_Max
};

enum ECGReportSpeed         //报告波形速度
{
    ECGReportSpeed_25 = 0,
    ECGReportSpeed_50,
    ECGReportSpeed_Max
};

enum ECGReportLayoutIndex           //报告打印格式
{
    ECGReportLayout_3x4_1 = 0,
    ECGReportLayout_3x4_3,
    ECGReportLayout_6x2,
    ECGReportLayout_6x2_1,
    ECGReportLayout_12x1,
    ECGReportLayout_max
};

typedef enum
{
    NurseCall_PhyOnly,
    NurseCall_TecOnly,
    NurseCall_PhyAndTec,
}NurseCallAlarmType;

typedef enum
{
    NurseCall_HighOnly,
    NurseCall_HighAndMed,
    NurseCall_All,
}NurseCallAlarmPriority;

typedef enum
{
    NurseCall_Continuous,
    NurseCall_Pulse,
}NurseCallSignalType;

typedef enum
{
    NurseCall_NormallyOpen,
    NurseCall_NormallyClosed,
}NurseCallContactType;

typedef enum
{
    AlarmSound_ISO,
    AlarmSound_Mode1,
}AlarmSoundType;

typedef enum
{
    Lock_Sight,
    Lock_Voice,
}LockType;

typedef enum
{
    MiniTrendsDuration_30Min = 0,
    MiniTrendsDuration_1Hrs,
    MiniTrendsDuration_2Hrs,
    MiniTrendsDuration_4Hrs,
    MiniTrendsDuration_6Hrs,
    MiniTrendsDuration_8Hrs,
    MiniTrendsDuration_Max,
}MiniTrendsDuration;

// 监护仪当前的系统模式
typedef enum
{
    SystemMode_Monitor = 0,
    SystemMode_Demo = 1,
    SystemMode_Privary = 2,
    SystemMode_Standby = 3,
    SystemMode_Max
} SystemMode;

typedef enum
{
    OxyCRGDuration_1Min = 0,
    OxyCRGDuration_2Min,
    OxyCRGDuration_4Min,
    OxyCRGDuration_8Min,
    OxyCRGDuration_Max,
}OxyCRGDuration;

//ECG导联标准
typedef enum
{
    ECG_LeadStandard_AHA = 0,       //美标
    ECG_LeadStandard_IEC,           //欧标
    ECG_LeadStandard_MAX
}ECGLeadStandard;

typedef enum
{
    Cfg_LoadCurrent,
    Cfg_SetDefault,
    Cfg_Import,
    Cfg_Export,
    Cfg_Delete,
}CfgSelectType;

// 待机的模式
typedef enum
{
    StandbyMode_Normal = 0,
    StandbyMode_DischargePatient = 1,
} StandbyMode;

// 系统关键事件列表,顺序不能变,会影响数据库的存储
typedef enum
{
    SystemEvent_Start = 0,
    SystemEvent_ToMonitor = 0,
    SystemEvent_ToStandby = 1,
    SystemEvent_ToDemo = 2,
    SystemEvent_ToPrivary = 3,
    SystemEvent_PowerOn = 4,
    SystemEvent_PowerOff = 5,
    SystemEvent_P1Plugin = 6,
    SystemEvent_P1Pullout = 7,
    SystemEvent_end,
} SystemEvent;

typedef enum
{
    OxyCRGTrendHRGroup_HR,
    OxyCRGTrendHRGroup_HR_RR,
    OxyCRGTrendHRGroup_Max,
}OxyCRGHRTrendGroup;

typedef enum
{
    OxyCRGTrendSpo2Group_Spo2,
    OxyCRGTrendSpo2Group_Spo2L,
    OxyCRGTrendSpo2Group_SPo2_Spo2L,
    OxyCRGTrendSpo2Group_Max,
}OxyCRGSpo2TrendGroup;


typedef enum
{
    Power_Ac,
    Power_Dc,
} PowerSupplyMode;

typedef enum
{
    Battery_Fault,
    Battery_Charging,
    Battery_ChargingCompleted,
    Battery_Discharging,
} BatteryStatus;

typedef enum
{
    Battery_Empty,
    Battery_OneCell,
    Battery_TwoCell,
    Battery_ThreeCell,
    Battery_FourCell,
} BatteryPower;

typedef enum
{
    Usb_Normal,
    Usb_Unmount,
    Usb_NoConfig,
} UsbError;

typedef enum
{
    Request_Null,
    Request_NibpMeasure,
    Request_AlarmClose,
    Request_AlarmPause,
    Request_VolumeIncrease,
    Request_VolumeDecrease,
    Request_AdmitNewPatient,
    Request_BrightnessBrighten,
    Request_BrightnessDarken,
    Request_WaveFreeze,
    Request_SwitchBigFontScreen,
    Request_SwitchStandScreen,
    Request_SwitchOxygen,
    Request_RecordRealtimeWave,
    Request_RecordTrendData,
    Request_NightMode,
    Request_Poweroff,
    Request_Screenshot,
} BoardRequestType;

typedef enum        //网络类型
{
    NetWorkType_Lan,
    NetWorkType_Wlan,
    NetWorkType_max
} NetWorkType;

typedef enum
{
    Overview,
    Detail,
} DataType;

typedef enum
{
    Voice_Wait,
    Voice_Online,
    Voice_Offline,
}VoiceStatus;

}


Q_DECLARE_METATYPE(Gps::WaveSpeed)
Q_DECLARE_METATYPE(Gps::StatusBarIcon)
Q_DECLARE_METATYPE(Gps::LockType)
Q_DECLARE_METATYPE(Gps::CfgSelectType)
Q_DECLARE_METATYPE(Gps::SystemEvent)
Q_DECLARE_METATYPE(Gps::StandbyMode)
Q_DECLARE_METATYPE(Gps::UsbError)
Q_DECLARE_METATYPE(Gps::DataType)

#endif // VALUEID

