#ifndef MODULERESP_H
#define MODULERESP_H

#include "imodule.h"
#include "moduledata.h"
#include "valueid.h"

struct RespParam : public ModuleParam
{
public:
    RespParam(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~RespParam();

    inline bool isValid() const
    {
        return m_RespRate != INVALID_DATA;
    }

    virtual void reset();

    virtual QList<ParamPoint> toList();

public:
    // 呼吸率;测量范围0~150,255为无效数据
    int m_RespRate = INVALID_DATA;

    // 呼吸率数值的具体来源; 用于在界面上的参数区进行显示;
    // 来源于ECG,并不等价于受LABEL_ECG上下线的影响; 应理解为呼吸板的ECG
    LabelID  m_RespSource = LABEL_CO2;
};

struct RespStatus : public ModuleStatus
{
public:
    RespStatus(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual void reset();

    virtual ~RespStatus();

    inline bool hasError()
    {
        return (errorCode != 0 || m_LeadsOff != 0);
    }

public:
    quint8  m_LeadsOff = 0;                                         // 呼吸脱落标志位 1:呼吸脱落, 0:呼吸不脱落
    quint8  m_Apnea = 0;                                            // 1:窒息; 0:不窒息;
    quint8  m_HeartBeatDisturbSig = 0;                              // 心动干扰标志; 0:正常, 1:呼吸受心动干扰;
    quint8  m_RespFilterSwitchSig = 0;                              // 0:呼吸滤波关;   1:呼吸滤波开;
    quint8  m_ModuleLabel;                                          // 模块标名
    bool    m_SenorHasConnectedBody = false;                        // 判断是否连接过人体,处理报警时有所差异;
    Gps::RespLeadType m_RespLeadType = Gps::RESP_Lead_RA_LL;        // 呼吸导联类型
    Gps::RespApneaDelayTime m_ApneaAlmTime = Gps::RESP_ApneaDelay_20;
//    quint8  m_UpdateRR = 1;                                       // 呼吸率是否有更新(保留)
//    quint8  m_Startbreath = 1;                                    // 1:开始呼气(保留)
//    quint8  m_StartInhale = 1;                                    // 1:开始吸气(保留)
};

struct RespWaveUnit
{
    RespWaveUnit()
    {
        wave = 0;
        bValid = false;
    }

    int16_t wave;

    /*
     * @brief 标识波形数据的有效性   true:有效    false:无效
     * 以导联脱落或者呼吸率为255时视为呼吸波形数据无效,判断条件不包括呼吸模块上传的窒息标志位!!!
    */
    bool bValid;
};

struct RespWave : public ModuleWave<RespWaveUnit>
{
public:
    RespWave(int size = 0, ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual void reset();

    virtual ~RespWave();

    virtual QList<WavePoint> toList(WaveID id = WAVE_NULL);
};

struct RespModuleData : public ModuleData
{
public:

    explicit RespModuleData(ModuleID module = MODULE_ID_RESP, LabelID label = LABEL_RESP);

    virtual ~RespModuleData();

    void reset();

    RespParam                    *m_pParam;
    RingBuffer<RespWaveUnit>     *m_pWave;
    RespStatus                   *m_pStatus;
    ModuleInfo                  m_ModuleInfo;
};

class ModuleResp : public IModule
{
public:
    explicit ModuleResp(ModuleProperty property, QObject *parent = nullptr);

    virtual ~ModuleResp();

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
    RespModuleData          *m_pRespModuleData;
    modulepackets           m_modulePacket;
};

Q_DECLARE_METATYPE(RespModuleData *)

#endif // MODULERESP_H
