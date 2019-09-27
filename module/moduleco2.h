#ifndef MODULECO2_H
#define MODULECO2_H

#include "imodule.h"
#include "moduledata.h"
#include "valueid.h"

struct CO2Param : public ModuleParam
{
public:
    CO2Param(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~CO2Param();

    virtual void reset()
    {
        breathRate = INVALID_DATA;
        InHalePot  = INVALID_DATA;
        ExHalePot  = INVALID_DATA;
        InHaleTime = INVALID_DATA;
        ExHaleTime = INVALID_DATA;
        momenttary = INVALID_DATA;
        BreathFlag = false;
    }

    virtual QList<ParamPoint> toList();

public:
    int breathRate = INVALID_DATA;       //呼吸率
    int InHalePot  = INVALID_DATA;       //吸入CO2 浓度
    int ExHalePot  = INVALID_DATA;        //呼出CO2浓度
    int InHaleTime = INVALID_DATA;       //吸气时间s
    int ExHaleTime = INVALID_DATA;       //呼气时间s
    int momenttary = INVALID_DATA;       //瞬时浓度
    bool BreathFlag = false;
};

struct CO2Status : public ModuleStatus
{
    enum
    {
        CO2_Mainstream = 0,
        Co2_Sidestream,
    };

public:
    CO2Status(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual void reset();

    inline bool hasError()
    {
        bool ret = false;
        if (priorityStatus != 0 || sensorError != 0 || hardwareError != 0)
            ret = true;
        return ret;
    }

public:
    //C5 CO2 模块
    //Co2 Status
    int  nackError = 0 ;             //NACK ERROR
    quint8 sensorOff = 0;             //探头脱落
    quint8 haveConnectedBody = 0;     //0 没有连接过人体 1 连接过人体
    quint8 apnea = 0;                 //窒息状态
    quint8 hardwareState = 0;         //硬件状态
    quint8 priorityStatus = 0;            //优先级异常状态
    quint8 noBreathDetected = 0;      //呼吸标志 1为未检测到呼吸 1未检测到
    quint8 inSleepMode = 0;           //1:睡眠模式  0:工作模式
    quint8 notReadyZero = 0;          //是否可以校零 1表示未准备好校零
    quint8 co2OutOfRange = 0;         //CO2超范围
    quint8 breathDetected = 0;        //呼吸标志 1为检测到呼吸 0未检测到
    quint8 checkAdapter = 0;          //检查适配器
    quint8 negativeCo2Error = 0;      //负CO2错误
    quint8 sourceCurrentStatus = 0;   //电流状态 0 正常 1 等待电流稳定 2 源电流不稳定 3 源电流限制错误  3和4 属于硬件错误
    quint8 compensationNotYetSet = 0; //未初始化
    quint8 calibrationStatus = 0;     //校零状态过程 0 无校零正在进行 1正在校零 2 需要校零 3校零错误
    quint8 temperatureStatus = 0;     //传感器加热状态
    quint8 eepromError = 0;           //EEPROM checksum faulty
    quint8 hardwareError = 0;         //硬件错误
    quint8 pumpOff = 0;               //泵关闭
    quint8 checkSampleLine = 0;       //检查取样线
    quint8 pumpLifeExceeded = 0;      //泵超过使用寿命
    quint8 sampleLineDisconnected = 0;

//masimo co2
    //sensor regs
    quint8 sensorMode = 2;            //0=self test   1=sleep   2=measurement
    quint8 o2Compensation = 0;        //氧气补偿  0-115vol%
    quint8 sensorError = 0;           //bit0=1:softwave error   bit1=1:hardwave error  bit2=1:motor error  bit3=1:factory error
                                      //bit4=1:主流：N/A  旁流：O2 error
    quint8 adapterStatus = 0;         //BIT0 = 1:(MIAN:replace adapter Side:Sampling line clogged)
                                      //BIT1 = 1:(MIAN:no adapter Side:no Sampling line)
                                      //BIT2 = 1:(MIAN:O2 port failure  Side:N/A)
    quint8 dataValid = 0;             //详见手册 注：BIT6=1, zero required!
    quint8 nobreathsTimeout = 20;     // 20 - 60s

    //configdata
    quint8 sensorCfgRegister0 = 0;
    quint8 sensorCfgRegister1 = 0;
    quint8 sensorHardwareRevision = 0;
    quint16 sensorSoftwareRevision = 0; //byte 2 =hi byte3 =lo
    quint8 streamModel = Co2_Sidestream;      //0旁流还是1主流
    quint8 commProtocolRevision = 0;

    //serviceData
    quint32 sensorSNNumber = 0;              //byte0:mid byte byte1:low byte byte4 hi byte
    quint8  n2oCompensation = 0;
    quint8  serviceStatusRegister = 0;
    quint8  atmPressure = 0;             //单位hPa   即 (atmpressure - 20) / 10 = value kpa, 例如： （254 - 20）/10 = 23.4kpa

//public:
//    //CPT CO2 模块
//    //status a
//    bool value_enable;
//    bool value_on;
//    bool adult;
//    bool paramcomms_on;
//    bool src_on;
//    bool pump_on;
//    bool filter_on;
//    //status b
//    bool over_flow;
//    bool no_adapter;
//    bool self_zero;
//    bool temp_unstable;
//    bool cal_mode;
//    bool run_mode;
//    bool standby_mode;
//    //status c
//    bool no_breath;
//    bool temp_low;
//    bool temp_high;
//    bool therm_err;
//    bool cal_err;
//    bool a2din_err;
//    bool a2dcomm_err;
//    //status d
//    bool bench_err;
//    bool occlusion;
//    bool no_tube;
//    bool eeprom_err;
//    bool ram_err;
//    bool rom_err;
//    bool boot_err;
//    bool invalid_cmd;
};


struct CO2WaveUnit
{
    CO2WaveUnit()
    {
        waveValue = INVALID_DATA;
        bExhale = false;
        bVaild = false;
    }

    uint16_t waveValue;
    bool  bExhale;      //是否呼气    1:呼气  false:吸气
    bool bVaild;        //是否有效
};

struct CO2Wave : public ModuleWave<CO2WaveUnit>
{
public:
    CO2Wave(int size = 0, ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~CO2Wave();

    virtual void reset()
    {

    }

    virtual QList<WavePoint> toList(WaveID id = WAVE_NULL);

//    virtual QByteArray toByteArray(WaveID wave = WAVE_NULL);
};

//struct Co2BarometricPre
//{
//    bool bMainStream;       //true:主流  false:旁流
//    unsigned char type;  // 1:barometric pressure 2:sample chamber pressure
//    int pressureValue;  //mmhg
//};

struct Co2GasCompensation
{
    int O2;
    int balance;
    int anesthetic;
};
Q_DECLARE_METATYPE(Co2GasCompensation)

//struct Co2BalanceGasCompensation //平衡气体补偿
//{
//    bool btps;                  //
//    bool H2O;
//    unsigned char N2O;
//    unsigned char O2;
//    unsigned char He;
//};

//struct Co2ModuleData
//{

//    Co2BarometricPre barometric;        //大气压
//    unsigned short   gainCon;        //增益气体浓度
//    unsigned short   flowRate;      //流量 CC/min
//    bool             pumpOn;        //泵开关
//    int              runTime;           //运行时间
//    bool             bSysTemperature;   //系统温度
//    short            temperature;       //℃*10
//    Co2BalanceGasCompensation compensate;   //平衡气体补偿
//};

struct Co2ModuleData : public ModuleData
{
public:

    explicit Co2ModuleData(ModuleID module = MODULE_ID_CO2, LabelID label = LABEL_CO2);

    virtual ~Co2ModuleData();

    void reset();

    CO2Param                    *m_pParam;
    RingBuffer<CO2WaveUnit>     *m_pWave;
    CO2Status                   *m_pStatus;
    ModuleInfo                  m_ModuleInfo;
};

class ModuleCo2 : public IModule
{
public:
    explicit ModuleCo2(ModuleProperty property, QObject *parent = nullptr);

    virtual ~ModuleCo2();

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

    virtual void getData(QVariant &value);

protected:

private:
    Co2ModuleData               *m_pCo2ModuleData;
    modulepackets           m_modulePacket;
};

Q_DECLARE_METATYPE(Co2ModuleData *)

#endif // MODULECO2_H
