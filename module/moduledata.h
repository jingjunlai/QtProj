#ifndef MODULEDATA_H
#define MODULEDATA_H


#include "moduleid.h"
#include "ringbuffer.h"
#include "math.h"

#include <QtGlobal>
#include <QReadWriteLock>

#define NIBP_ERRDATA                0x8fff
#define INVALID_DATA                0xffff
#define SENSOROFF_DATA              0x6fff
#define S_INVALID_DATA              0x7fff
#define SPO2_INVALID_DATA           0xff
#define WAVE_POINTS_PER_SECOND_100  100
#define WAVE_POINTS_PER_SECOND_1000 1000

typedef enum modulestate        //模块状态
{
    Module_StateOnline = 0,
    Module_StateFault,
    Module_StateForbidden,
    Module_StateHandup,
    Module_StateMax
} ModuleState;

typedef enum probestate
{
    Probe_StateNormal = 0,                  //探头正常
    Probe_StateIncompatible,                //不兼容
    Probe_StateUnActivated,                 //未激活
    Probe_StateOverdued,                    //已过期
    Probe_StateBeingOutOfDate,              //即将过期
    Probe_StateMax
} ProbeState;

typedef enum : int                  //模块版本信息ID (待扩充)
{
    ModuleID_HardVer = 0,
    ModuleID_SoftVer,
    ModuleID_Algorithm,
    ModuleID_Firmware,
    ModuleID_SysSerialNum,          //系统序列号
    ModuleID_SensorSerial,          //传感器序列号
    ModuleID_Max
} ModuleVerID;

typedef struct probeinfo            //探头信息
{
    QString ProbeActivationTime = "unknown";  //探头激活时间
    uint32_t  ProbeUsedCount = 0;             //探头使用次数
    uint32_t  ProbeUsedTotalTime = 0;         //探头使用总时间min
    QString ProbeUniqueID = "unknown";        //探头唯一ID
} ProbeInfo;

typedef struct modulepackets
{
    quint8   Count = 0;         //算1秒的包数
    quint64  StatusPackets = 0;
    quint64  ParamPackets = 0;
    quint64  WavePackets = 0;
    quint64  DebugPackets = 0;
} ModulePackets;

struct WavePoint
{
    char extend = 0;         //扩展数据  用于各个参数显示附加数据bit0~bit7
    bool valid = false;
    long data = 0;
};

struct ParamPoint
{
    quint8 label = 0;
    quint8 subid = 0;
    qint32 data = 0;
};

typedef struct moduleinfo
{
    QMap<ModuleVerID, QString> moduleVerInfo;   //版本信息
    ProbeInfo probe;                            //探头信息
    modulepackets packets;                      //包数统计
} ModuleInfo;

class ModuleParamBase;
class ModuleWaveBase;
struct ModuleData
{
public:
    explicit ModuleData(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~ModuleData();

    /*
     * @warning: 请注意,拷贝构造,以及等号重载,在子类里应避免继承,若是使用请按照以下格式来编写子类
     * class childModuleData : public ModuleData
     * {
     * public:
     *     clildModuleData(const childModuleData &other) : ModuleData(other)
     *     {
     *         xxx;
     *     }
     *     childModuleData &operator= (const childModuleData &other)
     *     {
     *         ModuleData::operator=(other);
     *         xxx;
     *         return *this;
     *     }
     * }
     * 子类的等号重载使用,需要转换成相应的子类后,才能使用
     * @example: *dynamic_cast<childModuleData*>(basepoint) = child;
     */
    ModuleData(const ModuleData &other);

    ModuleData &operator= (const ModuleData &other);

    // @warning: 本接口主要用于数据复位,并不是用于初始化变量
    // 变量初始化应该请参考下方param的初始化方式,若是变量需要根据外部设置,可考虑构造传参,也可考虑设计接口传参
    virtual void reset();

    ModuleID moduleId() const;

    LabelID labelId() const;

    virtual void clear() {}

public:
//    ModuleParamBase *param = Q_NULLPTR;
//    ModuleWaveBase  *wave = Q_NULLPTR;

private:
    ModuleID mModuleId;
    LabelID mLabelId;
};

struct ModuleParamBase : public ModuleData
{
public:
    explicit ModuleParamBase(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    /**
     * @brief toList 将参数的数据结构,封装成ParamPoint的列表
     * @return 封装后的数据列表
     */
    virtual QList<ParamPoint> toList();
};

struct ModuleWaveBase : public ModuleData
{
public:
    explicit ModuleWaveBase(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    /**
     * @brief toList 将波形的数据结构,封装成WavePoint的列表
     * @param id 期望获取波形数据的ID,若是不是label对多wave的情况,不需要设置
     *           如ECG,需要获取对应的波形数据,则需要设置
     * @return 封装后的数据列表
     */
    virtual QList<WavePoint> toList(WaveID id = WAVE_NULL);
};

struct ModuleParam : public ModuleParamBase
{
public:
    ModuleParam(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~ModuleParam();
};


class ModuleStatus : public ModuleData
{
public:
    ModuleStatus(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~ModuleStatus();
public:
    ModuleState moduleState;        //模块状态  ModuleState
    ProbeState probeState;          //探头状态      enum ProbeState
    int physicalID;                 //模块物理ID 32位
    short powerFrequency;           //工频滤波  EcgPowerFrequency
    short patientType;              //病人类型  EcgPatientType
    short errorCode;                //错误代码
};

template<typename T>
struct ModuleWave : public ModuleWaveBase
{
public:
    ModuleWave(int size = 0, ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~ModuleWave();

    void setCapacity(int size);

    int capacity() const;

    int size() ;//const;

    bool Flag() ;//const;

    int unUsedSize() const;

    void append(const T *d, int len);

    void remove(int index, int count);

    T *data();

    T *endData();

    virtual void clear();

protected:
    void expandTo(int len);

protected:
    int             waveCounts;
    int             capacitySize;
    T               *buffer;
    QReadWriteLock  m_lock;

public:
    bool        updateFlag = false;


};


template<typename T>
ModuleWave<T>::ModuleWave(int size, ModuleID module, LabelID label)
    : ModuleWaveBase(module, label)
    , waveCounts(0)
    , capacitySize(size < 0 ? 0 : size)
    , buffer(nullptr)
{
    if (capacitySize)
    {
        buffer = new T[capacitySize];
    }
}

template<typename T>
ModuleWave<T>::~ModuleWave()
{
    if (buffer != Q_NULLPTR)
        delete [] buffer;
    buffer = Q_NULLPTR;
}

template<typename T>
void ModuleWave<T>::setCapacity(int size)
{
    if (size <= 0)
        return;

    int mark = 1;
    int powers = 1;
    while ((mark <<= 1) < size)
    {
        powers++;
    }
    size = ::pow(2, powers);
    if (size > capacitySize)
        expandTo(size);
}

template<typename T>
int ModuleWave<T>::capacity() const
{
    return capacitySize;
}

template<typename T>
int ModuleWave<T>::size()
{
//    QReadLocker locker(&m_lock);
    return waveCounts;
}

template<typename T>
bool ModuleWave<T>::Flag()
{
//    QReadLocker locker(&m_lock);
    return updateFlag;
}

template<typename T>
int ModuleWave<T>::unUsedSize() const
{
    return capacity() - size();
}

template<typename T>
void ModuleWave<T>::append(const T *d, int len)
{
    if (!d || len < 0)
        return;

    if (size() + len > capacity())
    {
        setCapacity(size() + len);
    }
    T *dd = endData();
    for (int i = 0; i < len; i++)
        dd[i] = d[i];
    waveCounts += len;

//    QWriteLocker locker(&m_lock);

//    if (len > capacity())
//    {
//        setCapacity(len);
//    }

//    for (int i = 0; i < len; i++)
//        buffer[i] = d[i];
//    waveCounts = len;
//    updateFlag = true;
}

template<typename T>
void ModuleWave<T>::remove(int index, int count)
{
    if (count <= 0)
        return;

    if (index < 0 || index >= waveCounts)
        return;

    int start = index + count;
    int restcount = waveCounts - start;
    if (restcount > 0)
    {
        for (int i = 0; i < restcount; i++)
        {
            buffer[index + i] = buffer[start + i];
        }
        waveCounts -= count;
    }
    else
    {
        waveCounts = index;
    }
}

template<typename T>
T *ModuleWave<T>::data()
{
//    QReadLocker locker(&m_lock);
//    updateFlag = false;
    return buffer;
}

template<typename T>
T *ModuleWave<T>::endData()
{
    return buffer + waveCounts;
}

template<typename T>
void ModuleWave<T>::clear()
{
    waveCounts = 0;
}

template<typename T>
void ModuleWave<T>::expandTo(int len)
{
    if (len <= 0 || len <= capacitySize)
        return;

    T *d = new T[len];
    for (int i = 0; i < capacitySize; i++)
    {
        d[i] = buffer[i];
    }
    if (buffer)
        delete [] buffer;
    buffer = d;
    capacitySize = len;
}

// 定义趋势数据的数据结构
typedef QMap<LabelID, ModuleData *> DataCacheUnit;

Q_DECLARE_METATYPE(ModuleData)

#endif // MODULEDATA_H
