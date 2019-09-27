#ifndef MODULEECG_H
#define MODULEECG_H

#include "imodule.h"
#include "moduledata.h"
#include "valueid.h"


#define HR_INVALID 511
#define ST_INVALID 32767
#define PVC_INVALID 32767
#define QRS_THRESHOLD_WAVE_COUNT 500 //5秒缓存数据 QRS阈值调节

enum ECG_ARRErrorCode  //心率失常代码索引定义
{
    ARR_ErrorCodeNormal = 0,    //0 正常心律
    ARR_ErrorCodeAsy,           //1 ASY,停搏
    ARR_ErrorCodeFib,           //2 FIB，室颤
    ARR_ErrorCodeRun,           //3 RUN,三个或四个连发室早（RUN）
    ARR_ErrorCodeCpt,           //4 CPT, 二连发室早（CPT）
    ARR_ErrorCodeBgm,           //5 BGM, 室早二联律
    ARR_ErrorCodeTgm,           //6 TGM, 室早三联律
    ARR_ErrorCodeRot,           //7 ROT, R on T
    ARR_ErrorCodeVpb,           //8 VPB, 单个室早
    ARR_ErrorCodeTachy,         //9 TACHY, 室上速
    ARR_ErrorCodeBrady,         //10 BRADY, 室缓
    ARR_ErrorCodeMis,           //11 MIS,漏搏
    ARR_ErrorCodeAFib,          //12 房颤
    ARR_ErrorCodeAFibEnd,       //13 房颤停止
    ARR_ErrorCodePnc,           //14 PNC, 起搏器未俘获
    ARR_ErrorCodePnp,           //15 PNP 起搏器未起搏
    ARR_ErrorCodePac,           //16 PAC，未用
    ARR_ErrorCodeNos = 18,      //18 NOS，噪声
    ARR_ErrorCodeLrn = 20,      //20 LRN,QRS学习
    ARR_ErrorCodeSwitch,        //21 ARR_DETECT_OFF，ARR分析关
    ARR_ErrorCodeVta = 30,      //30 VTA，室速
    ARR_ErrorCodePvch,          //31 PVCH
    ARR_ErrorCodeNsvt,          //32 NSVT,
    ARR_ErrorCodeEtac,          //33 ETAC,
    ARR_ErrorCodeEbrd,          //34 EBRD,
    ARR_ErrorCodePause,         //35 PAUSE,
    ARR_ErrorCodeIRHR,          //36 IRHR,
    ARR_ErrorCodeVBRD,          //37 VBRD,
    ARR_ErrorCodeVrhy,          //38 VRHY,
    ARR_ErrorCodeIRHRE,         //39 不规则节律停止
    ARR_ErrorCodePauseh,        //40 PAUSEH
    ARR_ErrorCodeMax
};

typedef enum ecgchannelindex
{
    ECG_Channel_I = 0,
    ECG_Channel_II,
    ECG_Channel_V1,
    ECG_Channel_V2,
    ECG_Channel_V3,
    ECG_Channel_V4,
    ECG_Channel_V5,
    ECG_Channel_V6,
    ECG_ChannelMax
}EcgChannelIndex;


struct STBaseLineData
{
    void reset()
    {
        for (int i = 0; i < 12; i++)
        {
            baseSTValue[i] = INVALID_DATA;
        }
        hasSTBaseLine = false;
    }
    int baseSTValue[12];                        //基线ST值
    int baseSTWave[12][250];                    //基线STwave
    bool hasSTBaseLine;                         //基线是否有效
    QString STBaseLineUpdateTime;               //ST基线更新时间
};

typedef struct
{
    void reset()
    {
        valid = false;
        tempLength = 0;
        qtValue = INVALID_DATA;
        qtcBazett = INVALID_DATA;
        qtcHodges = INVALID_DATA;
        qtcFridericia = INVALID_DATA;
        qtcFramingham = INVALID_DATA;
        qtHR = INVALID_DATA;
    }
    bool valid;
    int templetData[12][1500];	                    // 12导联波形模板数据
    int tempLength;									// 模板数据波形长度,每个导联长度相同
    int pStart, pEnd;								// 复合波P波的平均起点和终点
    int qrsStart, qrsEnd;							// 复合波QRS波群的平均起点和终点
    int tStart, tEnd;								// 复合波T波的平均起点和终点
    int rPoint;										// 复合波R波顶点
    int qrsMorph;									// 复合波模板类型
    int qtValue;
    int qtcBazett;
    int qtcHodges;
    int qtcFridericia;
    int qtcFramingham;
    int qtHR;
} QtBaseValue;

struct QTBaseLineData
{
    void reset()
    {
        qtBaseLineUpdateTime = "";
        baseValue.reset();
    }
    QtBaseValue     baseValue;
    QString         qtBaseLineUpdateTime;               //QT基线更新时间
};

struct QRSThresholdWave //QRS 设置调节阈值时的显示波形
{
    int ecgWave[2] = {0};       //主要计算导联+辅助计算导联 0:主要计算导联 1:辅助计算导联
};

enum ECGModuleEventID //模块执行动作ID
{
    ECG_ModuleEventARRLearn = 0,    //ARR学习
    ECG_ModuleEventSTBaseLine,      //ST基线设置
    ECG_ModuleEventQTBaseLine,      //QT基线设置
    ECG_ModuleEventMax
};

struct ECGParam : public ModuleParam
{
public:
    ECGParam(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~ECGParam();

    virtual void reset();

    virtual QList<ParamPoint> toList();

public:
    //OUT 非实时心电数据
    unsigned short hr = INVALID_DATA;       //  511 表示无效
    unsigned char hrConfidenceLevel;        // 心率可信度水平，0~100%，暂时用信号质量代替
    unsigned char arrCode = 0;              //默认为正常
//    int stValue[12];                        // 标准增益，10^4, +-2 mV
    int pvcMin;
    int pvcCnt;
//    int sqi[12];                            // 0~100%          //信号质量
//    int arrWave[8*1000];                    //  主要导联的数据，长度8秒，从心律失常发生往前后各四秒
    unsigned char stUpdate;                 // 1=更新，2=ST无法更新，0=中间状态
//    int stWave[12][250];                    // 前面400 ms, 后面600 ms
    int newArrEvent;		                // 新的心律失常事件标记
    QString arrInfo;
    int autoJPos;                           //自动时J点输出值
    int autoISOPos;                         //自动时ISO点输出值
    bool bFindPace;                         //是否检测到起搏信号
    unsigned short qtcValue;                //qt计算值,跟据软件选择保存(Hodges,Bazett,Fridericia,Framingham)其中之一数据
    unsigned short baseQtcValue;
    unsigned char slead1,slead2;            // 自动选择了哪两个导联。 255=无效值
    //EcgDiagLibOutput measureOut;
//    STBaseLineData stBase;
//    QTBaseLineData qtBase;
//    QList<QRSThresholdWave *> qrsThresholdWave;         //qrs阈值设置时 5s缓存数据
};

struct ECGStatus : public ModuleStatus
{
public:
    ECGStatus(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual void reset()
    {
        leadType = 0;
        calLead  = 0;
        sensorOff = false;
        memset(&electrodeOff, 0, sizeof(electrodeOff));
    }

    inline bool hasError()
    {
        return errorCode != 0;
    }

    short           leadType;           //导联类型      0：5导 1：3导 2：12导 3：6 导
    short           calLead;            //计算导联
    bool            sensorOff = false;  //导联脱落标志
    bool            electrodeOff[Gps::EcgElecMax] = {false};    //导联脱落的导联
    bool            electrodePolar[Gps::ECG_LEAD_MAX] = {false};
    bool            leadVaild[Gps::ECG_LEAD_MAX] = {false};     //各导有效性
    Gps::EcgMode    workMode = Gps::ECG_ModeMonitor;

};


struct ECGWaveUnit
{
    ECGWaveUnit()
    {
        memset(ecg, 0, sizeof(ecg));
        wvFlag = false;
        memset(vaild, false, sizeof(vaild));
        polarizationFlag = 0;
        paceFlag = 0;
        autoPace = 0;
        rFlag = 0;
    }

    //OUT 实时心电数据
    int ecg[12];                      // 滤波后波形，变成12导联的波形数据输出
    short wvFlag;                       // 有效状态           //bit 0 ~ bit 11  I~V6 导联有效状态
    bool vaild[12];                     //有效状态
    short polarizationFlag;             // 极化状态   同上
    short paceFlag;		                // 起搏标志，12通道  同上
    unsigned char autoPace;			    // 自动起搏提示，1=检测有起搏，0=未检测到起搏
    unsigned char rFlag;
};

struct ECGWave : public ModuleWave<ECGWaveUnit>
{
public:
    ECGWave(int size = 0, ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~ECGWave();

    virtual void reset() {}

    virtual QList<WavePoint> toList(WaveID wave = WAVE_NULL);
};

struct EcgModuleData : public ModuleData
{
public:

    explicit EcgModuleData(ModuleID module = MODULE_ID_ECG, LabelID label = LABEL_ECG);

    virtual ~EcgModuleData();

    void reset();

    ECGParam                    *m_pParam;
    RingBuffer<ECGWaveUnit>     *m_pWave;
    ECGStatus                   *m_pStatus;
    ModuleInfo                  m_ModuleInfo;
};

class ModuleEcg : public IModule
{
    Q_OBJECT

public:
    explicit ModuleEcg(ModuleProperty property, QObject *parent = nullptr);

    virtual ~ModuleEcg();

    /**
     * @brief unpackModuleData 解析ECG数据
     * @param packet
     */
    virtual void unpackModuleData(const QByteArray& packet);

    /**
     * @brief setConfig
     * @param id
     * @param value
     * @return
     */
    virtual bool setConfig(int id, const QVariant &value);

    /**
     * @brief getConfig
     * @param id
     * @param value
     * @return
     */
    virtual bool getConfig(int id, QVariant &value);

    virtual void setData(const QVariant &value);

    virtual void setArrData(const QVariant &value);

    virtual void getData(QVariant &value);

protected:

private:

//    ECGParam            *m_pEcgParam;
//    ECGWave             *m_pEcgWave;
//    ECGStatus           *m_pEcgStatus;
    EcgModuleData           *m_pEcgModuleData;
    modulepackets           m_modulePacket;

};

Q_DECLARE_METATYPE(EcgModuleData *)

#endif // MODULEECG_H
