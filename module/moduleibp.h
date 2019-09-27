#ifndef MODULEIBP_H
#define MODULEIBP_H

#include "imodule.h"
#include "moduledata.h"
#include "valueid.h"

#include <QDateTime>

struct IbpParam : public ModuleParam
{
public:
    IbpParam(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~IbpParam();

    virtual void reset();

    virtual QList<ParamPoint> toList();

    inline bool isValid() const
    {
        return (sys != INVALID_DATA
                || dia != INVALID_DATA
                || mean != INVALID_DATA);
    }

    inline bool prIsValid() const
    {
        return pr != INVALID_DATA;
    }

    inline bool ppvIsValid() const
    {
        return ppv != INVALID_DATA;
    }

public:
    int sys = INVALID_DATA;  //收缩压
    int dia = INVALID_DATA;  //舒张压
    int mean = INVALID_DATA; //平均压
    int pr = INVALID_DATA;   //脉率
    int ppv = INVALID_DATA;  //脉压差
};

typedef enum
{
    IBP_ZeroSucceed = 0,
    IBP_ZeroFailure,
    IBP_ZeroInProgress,
    IBP_ZeroRequired,
    IBP_ZeroStateMax,
}IbpZeroState;

typedef enum
{
    IBP_CalibrationSucceed = 0,
    IBP_CalibrationFailure,
    IBP_CalibrationInProgress,
    IBP_CalibrationMax,
}IbpCalibraionState;

struct IbpStatus : public ModuleStatus
{
public:
    IbpStatus(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~IbpStatus();

    virtual void reset();

     bool hasError() const;

public:
    uint8_t sensorOff = 0;                     //探头是否脱落 0--正常, 1--脱落
    uint8_t catheterOff = 0;                   //导管是否脱落 0--正常, 1--脱落
    uint8_t zeroState = 0;                     //校零状态 0--成功, 1--失败, 2--正在校零 , 3--需要校零
    uint8_t calibrationState = 0;              //压力校准状态 0--成功, 1--失败, 2--正在校零
    uint8_t filterState = 0;                   //IBP滤波状态 0--中, 1--弱, 2--强
    uint8_t averageTime = 0;                   //平均时间 0--普通, 1--快, 2--慢
    uint8_t ppvState = 0;                      //ppv状态 00, 01--不良信号, 02 --检查源
    uint8_t labelName;                         //模块标名
    uint8_t haveConnectedBody = 0;             //0--未连接过人体   1--连接过人体
    uint8_t showZeroSucceed = 0;               //0--不显示校零成功   1--显示校零成功
    uint8_t showCalibrateSUcceed = 0;          //0--不显示压力校准成功   1--显示压力校准成功
    QList<QDateTime> zeroSucceedTimes;         //校零成功的时间
    QList<QDateTime> calibrateSucceedTimes;    //压力校准成功的时间

};


struct IbpWaveUnit
{
    IbpWaveUnit()
    {
        wave = 0;
        rFlag = false;
        bVaild = false;
    }

    bool rFlag;
    int wave;
    bool bVaild;
};

struct IbpWave : public ModuleWave<IbpWaveUnit>
{
public:
    IbpWave(int size = 0, ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~IbpWave();

    virtual void reset();

    virtual QList<WavePoint> toList(WaveID wave = WAVE_NULL);
};

struct IbpModuleData : public ModuleData
{
public:

    explicit IbpModuleData(ModuleID module = MODULE_ID_IBP, LabelID label = LABEL_IBP_START);

    virtual ~IbpModuleData();

    void reset();

    IbpParam                    *m_pParam;
    RingBuffer<IbpWaveUnit>     *m_pWave;
    IbpStatus                   *m_pStatus;
    ModuleInfo                  m_ModuleInfo;
};

class ModuleIbp : public IModule
{
public:
    explicit ModuleIbp(ModuleProperty property, QObject *parent = nullptr);

    virtual ~ModuleIbp();

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
    IbpModuleData               *m_pIbpModuleData;
    modulepackets           m_modulePacket;
};

Q_DECLARE_METATYPE(IbpModuleData *)

#endif // MODULEIBP_H
