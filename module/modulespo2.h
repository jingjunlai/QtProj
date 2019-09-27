#ifndef MODULESPO2_H
#define MODULESPO2_H

#include "imodule.h"
#include "moduledata.h"
#include "valueid.h"

struct Spo2Param : public ModuleParam
{
public:
    Spo2Param(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~Spo2Param();

    virtual void reset();

    virtual QList<ParamPoint> toList();

public:
    //common
    LabelID m_Spo2Label;                 //spo2标明设定，有SPO2和SPO2_L
    uint8_t m_iConnectState = 0;         //判断是否连接过人体，处理报警的 时候是 不同的
    uint16_t m_iPrValue = INVALID_DATA;  //从血氧模块来的PR ,  脉率的有效测量范围是0-300，511 = 无效数据
    uint16_t m_iSpO2Value = INVALID_DATA;// 血氧值,   血氧值的测量范围是0-100；>100(255)表示无效数据
    uint16_t  m_iBreathRate = INVALID_DATA;       //血氧呼吸率
    uint16_t m_iPIValue = INVALID_DATA;  //PI值, 扩大10000倍上传，有效范围为0~20.000%   是%比的10000倍, 20% * 10000 = 2000上来
    uint8_t m_iPRUpdateTimeout = 1;      //1=脉率数值没有更新已超过25秒 0=脉率数值有更新
    uint8_t m_iSpO2UpdateTimeout = 1;    //1=血氧数值没有更新已超过25秒 0=血氧数值有更新
    uint8_t m_iIsUpdatePR = 1;
    uint8_t m_iIsUpdateSpO2 = 1;
    uint8_t m_iBloodColumnStrength = 0;  // 血柱强度（0-15）
    uint16_t m_iSignalQualityIndex = 0;  //信号质量指数    范围为0~100     单位为%，低于60%应交替显示“?”
    //blt spo2
    uint8_t m_iBltSpO2Type = 1;          // 0：表示模拟血氧1：表示数字血氧（主机不需要处理）
    uint8_t m_iIsWithResp = 0;           //0：表示不带呼吸 1：带呼吸  (保留)

    //masimo spo2

    //nellcor spo2

};

struct Spo2Status : public ModuleStatus
{
public:
    Spo2Status(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~Spo2Status();

    virtual void reset();

    inline bool hasError()
    {
        return (m_iSensorOff == 1 || m_iProbeLose == 1);
    }

public:
    //common
       uint8_t m_iSensorOff = 0;                //1= 探头脱落,   表示探头未接手指
       uint8_t m_iProbeLose = 0;                //1= 探头未连接, 表示探头故障或分体式探头未接至模块上
       uint8_t m_iSearchTooLong = 0;            //1 =脉搏信号弱（脉搏搜索时间太长）0 = 正常 ;Searching too long应为高级生理报警
       uint8_t m_iSearchingPulse = 0;           //1 = 正在搜索脉搏, Searching pulse，提示正在搜索脉搏
       uint8_t m_iMotionInterference = 0;       //1 = 有运动干扰          提示信息
       uint8_t m_iAverageTime = 0;              //平均时间：00：正常(约8秒) 01：快(约4秒) 02：慢(约16秒)
       uint8_t m_iSensitivityMode = 0;          //灵敏级别 灵敏度：0：普通，1高

       //blt spo2

       //masimo spo2

       //nellcor spo2

};


struct Spo2WaveUnit
{
    Spo2WaveUnit()
    {
        wave = 0;
        respwave = 0;
        rFlag = false;
        bValid = false;
    }

    bool rFlag;
//    int16_t wave;
    uint8_t wave;
//    uint16_t respwave;
    uint8_t respwave;
    uint8_t iBloodStrength;      // 血柱强度（0-15）
    bool bValid;
};

struct Spo2Wave : public ModuleWave<Spo2WaveUnit>
{
public:
    Spo2Wave(int size = 0, ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~Spo2Wave();

    virtual void reset() {}

    virtual QList<WavePoint> toList(WaveID wave = WAVE_NULL);
};

struct Spo2ModuleData : public ModuleData
{
public:

    explicit Spo2ModuleData(ModuleID module = MODULE_ID_SPO2, LabelID label = LABEL_SPO2);

    virtual ~Spo2ModuleData();

    void reset();

    Spo2Param                    *m_pParam;
    RingBuffer<Spo2WaveUnit>     *m_pWave;
    Spo2Status                   *m_pStatus;
    ModuleInfo                  m_ModuleInfo;
};

class ModuleSpo2 : public IModule
{
    Q_OBJECT

public:

    explicit ModuleSpo2(ModuleProperty property, QObject *parent = nullptr);

    virtual ~ModuleSpo2();

    virtual void unpackModuleData(const QByteArray &packet);

    virtual bool setConfig(int id, const QVariant &value);

    virtual bool getConfig(int id, QVariant &value);

    virtual void setData(const QVariant &value);

    virtual void getData(QVariant &value);

private:

//    Spo2Param            m_Spo2Param;
//    Spo2Wave             m_Spo2Wave;
//    Spo2Status           m_Spo2Status;

    Spo2ModuleData          *m_pSpo2ModuleData;
    modulepackets           m_modulePacket;
};

Q_DECLARE_METATYPE(Spo2ModuleData*)

#endif // MODULESPO2_H
