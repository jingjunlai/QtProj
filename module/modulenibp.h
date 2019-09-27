#ifndef MODULENIBP_H
#define MODULENIBP_H

#include "imodule.h"
#include "moduledata.h"
#include "valueid.h"
#include <QTime>

enum NibpModuleEventID  //模块执行动作ID
{
    NIBP_ModuleEventStart = 0,
    NIBP_ModuleEventStopAll,
    NIBP_ModuleEventSTAT,
    NIBP_ModuleEventVenipuncture,
    NIBP_ModuleEventAlarmDefault,
    NIBP_ModuleEventMax
};

struct NibpParam : public ModuleParam
{
public:
    NibpParam(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~NibpParam();

    virtual void reset();

    inline bool isValid() const
    {
        return (sys != INVALID_DATA
                && dia != INVALID_DATA
                && map != INVALID_DATA
                && pr != INVALID_DATA);
//                && sdp != INVALID_DATA);
    }

    virtual QList<ParamPoint> toList();

public:
    int sys = INVALID_DATA;  //收缩压
    int dia = INVALID_DATA;  //舒张压
    int map = INVALID_DATA;  //平均压
    int pr = INVALID_DATA;   //脉率
//    int sdp = INVALID_DATA;   //脉压差
    QTime measureTime; //测量结束时间
};

struct NibpStatus : public ModuleStatus
{
public:
    NibpStatus(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~NibpStatus();

    virtual void reset();

    inline bool hasError()
    {
        return !( (moduleState == 0)
               && (probeState == 0)
               && (errorCode == 0));
    }

public:
    uint8_t labelName;            //模块标名
    uint8_t normalValue = 0; // 0-正常上传结果;1-请求结果;

    uint8_t workMode = 0;  //模块上传的工作模式
    uint8_t lastWorkMode = 0;  //模块上传的工作模式
    uint8_t isWorking = 0;   //0静止,1正在工作
    uint16_t measureInterTime = 0; //0单次测量;其它时周期间隔时间;
    uint16_t cuffPressure = 0; //袖带压力
    //以下用于开发测试
    uint16_t protectPressure = 0; //过压保护压力
    //用于界面进度条的显示
    uint8_t currentWorkStateId; //工作状态ID
    uint8_t mainWorkStateId; //主工作状态ID
    uint8_t lastWorkStateId;  //上一个工作状态ID
    bool barDisplay = false;      //周期测量的进度条显示状态,true显示,false不显示
    uint16_t barTime = 0;      //周期测量的进度条倒计时
    uint16_t veniTime = 0;     //静脉穿刺需要倒计时
    bool stopAll = false;   //true停止所有测量，false有启动
    uint8_t squenceCurrentPhase; //序列测量的当前工作组
    uint8_t squenceNextPhase;  // 序列测量的下一个工作组
    uint16_t intervalTime;
    uint16_t paramInvalidCount;  //nibp参数失效计时
    uint8_t tecError;
    bool cycleStart = false;

};

struct NibpWaveUnit
{
    NibpWaveUnit()
    {
        wave = 0;
        bVaild = false;
    }

    uint16_t wave;
    bool    bVaild;
};

struct NibpWave : public ModuleWave<NibpWaveUnit>
{
public:
    NibpWave(int size = 0, ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~NibpWave();

    virtual void reset();

public:
};

struct NibpModuleData : public ModuleData
{
public:

    explicit NibpModuleData(ModuleID module = MODULE_ID_NIBP, LabelID label = LABEL_NIBP);

    virtual ~NibpModuleData();

    void reset();

    NibpParam                    *m_pParam;
    RingBuffer<NibpWaveUnit>     *m_pWave;
    NibpStatus                   *m_pStatus;
    ModuleInfo                  m_ModuleInfo;
};

class ModuleNibp : public IModule
{
public:
    explicit ModuleNibp(ModuleProperty property, QObject *parent = nullptr);

    virtual ~ModuleNibp();

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
    NibpModuleData              *m_pNibpModuleData;
    modulepackets           m_modulePacket;
};

Q_DECLARE_METATYPE(NibpModuleData *)

#endif // MODULENIBP_H
