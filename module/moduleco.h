#ifndef MODULECO_H
#define MODULECO_H

#include "imodule.h"
#include "moduledata.h"
#include "valueid.h"

#include <QDateTime>

struct COHemodynamicsResults
{
    uint16_t    co = INVALID_DATA;
    uint16_t    ci = INVALID_DATA;         // 心脏指数 // 单位: ml/min/m2

    uint16_t    sv = INVALID_DATA;         // 每搏射血量 // 单位: ml/beat
    uint16_t    svi = INVALID_DATA;        // 每搏射血指数 // 单位: ml/beat/m2
    uint16_t    svr = INVALID_DATA;        // 外周血管阻力 // 单位: dynes*sec/cm5
    uint16_t    svri = INVALID_DATA;       // 外周血管阻力指数 // 单位: (dynes*sec/cm5)*m2

    uint16_t    pvr = INVALID_DATA;        // 肺血管阻力 // 单位: dynes*sec/cm5
    uint16_t    pvri = INVALID_DATA;       // 肺血管阻力指数 // 单位: (dynes*sec/cm5)*m2

    uint16_t    lcw = INVALID_DATA;        // 左心做功 // 单位: Kg*m
    uint16_t    lcwi = INVALID_DATA;       // 左心做功指数 // 单位: Kg*m/m2

    uint16_t    rcw = INVALID_DATA;        // 右心做功 // 单位: Kg*m
    uint16_t    rcwi = INVALID_DATA;       // 右心做功指数 // 单位: Kg*m/m2

    uint16_t    lvsw = INVALID_DATA;       // 左室每搏功 // 单位: g*m/beat
    uint16_t    lvswi = INVALID_DATA;      // 左室每搏功指数 // 单位: g*m/beat/m2

    uint16_t    rvsw = INVALID_DATA;       // 右室每搏功 // 单位: g*m/beat
    uint16_t    rvswi = INVALID_DATA;      // 右室每搏功指数 // 单位: g*m/beat/m2
};

struct COStartMeasuringCmdData
{
    COStartMeasuringCmdData(float _catheter = 0.0, int16_t _ti = INVALID_DATA);

    uint8_t getCatheterHighData() const;

    uint8_t getCatheterLowData() const;

    uint8_t getTIHighData() const;

    uint8_t getTILowData() const;

protected:
    uint16_t    catheter = INVALID_DATA;
    int16_t     ti = S_INVALID_DATA;
};
Q_DECLARE_METATYPE(COStartMeasuringCmdData)

struct COParam : public ModuleParam
{
    COParam(ModuleID module = MODULE_ID_CO, LabelID label = LABEL_CO);

    uint16_t    tb = INVALID_DATA;
    int16_t     ti = S_INVALID_DATA;

    uint16_t    co = INVALID_DATA;
    uint16_t    ci = INVALID_DATA;         // 心脏指数 // 单位: ml/min/m2

    uint16_t    bsa = INVALID_DATA;

    // 存储CO结果包数据,以及CO的TB测量过程数据
    struct Result
    {
        QDateTime   dateTime;               // CO结果包的时间
        uint16_t    co = INVALID_DATA;
        uint16_t    ci = INVALID_DATA;
        QList<uint16_t> tbWaves;
    };
    QList<Result>   results;
    int resultsMaxCount = 6;                // 最大CO的测量数据集的长度

    // 需要时才计算血液动力学参数
    COHemodynamicsResults getHemodynamicsResults();
};

struct COStatus : public ModuleStatus
{
    COStatus(ModuleID module = MODULE_ID_CO, LabelID label = LABEL_CO);

    // CO测量状态
    enum class WorkerStatus : int
    {
        Ready = 0,                          // 可以开始测量
        Measuring = 1,                      // 正在进行测量
        Unstable = 2,                       // TB不稳定,还不能进行测量
        Max,
    };

    // 测量状态代码
    enum class MeasuringStatus : int
    {
        Vaild = 0,                          // 无错误,结果有效
        TBDataError = 1,                    // TB数据错误
        TIDataError = 2,                    // TI数据错误
        CatheterConstError = 4,             // 导管系数错误
        RangeExceeded = 8,                  // 测量超限
        ResetInjection = 16,                // 重新注射
        TimeOut = 32,                       // 测量超时
        Max
    };

    // 通道状态
    enum class ChannelStatus : int
    {
        Vaild = 0,                          // 测量有效
        SensorOff = 1,                      // 探头脱落
        OverHighRange = 2,                  // 测量范围超上限
        BelowLowRange = 3,                  // 测量范围超下限
        Max
    };

    // 测量模式状态
    enum class MeasureMode : int
    {
        Singal = 0,                         // 手动
        Continuous = 1,                     // 连续
        Max
    };

    // TI来源
    enum class TISource : int
    {
        Manual = 0,                         // 手动
        Auto = 1,                           // 自动
        Max
    };

    MeasuringStatus measuringStatus = MeasuringStatus::Vaild; // 测量代码

    WorkerStatus workingStatus = WorkerStatus::Ready; // 工作状态代码

    ChannelStatus tiChannelStatus = ChannelStatus::Vaild;

    ChannelStatus tbChannelStatus = ChannelStatus::Vaild;

    MeasureMode measureMode = MeasureMode::Singal;

    TISource tiSource = TISource::Manual;

    bool tiCheckError = false;

    bool tbCheckError = false;
};

struct CoModuleData : public ModuleData
{
public:

    explicit CoModuleData(ModuleID module = MODULE_ID_CO, LabelID label = LABEL_CO);

    virtual ~CoModuleData();

    void reset();

    COParam                     *m_pParam;
    COStatus                    *m_pStatus;
    ModuleInfo                  m_ModuleInfo;
};

class ModuleCo : public IModule
{
public:
    explicit ModuleCo(ModuleProperty property, QObject *parent = nullptr);

    virtual ~ModuleCo();

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
    CoModuleData                *m_pCoModuleData;
    modulepackets           m_modulePacket;
};

Q_DECLARE_METATYPE(CoModuleData *)

#endif // MODULECO_H
