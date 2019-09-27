#ifndef MODULETEMP_H
#define MODULETEMP_H

#include "imodule.h"
#include "moduledata.h"
#include "valueid.h"


struct TempParam : public ModuleParam
{
public:
    TempParam(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~TempParam();

    virtual void reset();

    virtual QList<ParamPoint> toList();

public:
    struct Data
    {
        uint8_t  subid = Sub_TempNull;          // 体温的子ID
        uint16_t temp = INVALID_DATA;           // 体温测量值,体温值*1000
    };

    Data        data[2];                        // 两个体温参数的值
    bool        dualTemp = false;

    uint16_t    deltaT = INVALID_DATA;          // 如果是双体温的话,表示体温1和2的差的绝对值
};

struct TempStatus : public ModuleStatus
{
public:
    TempStatus(ModuleID module = MODULE_ID_NULL, LabelID label = LABEL_NULL);

    virtual ~TempStatus();

    virtual void reset();

public:
    enum class MeasureStatus : int
    {
        MeasureValid = 0,   //测量正常
        Sensoroff,          //探头脱落
        OverDownLimit,      //超测量范围上限
        OverUpLimit,        //超测量范围下限
        Max
    };

    enum class Errcode
    {
        FirstCheckError = 0x40,
        SecondCkeckError,
        ErrMax,
    };

    MeasureStatus   measureStatus[2] = {MeasureStatus::MeasureValid, MeasureStatus::MeasureValid};
};

struct TempModuleData : public ModuleData
{
public:

    explicit TempModuleData(ModuleID module = MODULE_ID_TEMP, LabelID label = LABEL_TEMP_START);

    virtual ~TempModuleData();

    void reset();

    TempParam                    *m_pParam;
    TempStatus                   *m_pStatus;
    ModuleInfo                  m_ModuleInfo;
};


class ModuleTemp : public IModule
{
public:

    explicit ModuleTemp(ModuleProperty property, QObject *parent = nullptr);

    virtual ~ModuleTemp();

    virtual void unpackModuleData(const QByteArray &packet);

    virtual bool setConfig(int id, const QVariant &value);

    virtual bool getConfig(int id, QVariant &value);

    virtual void setData(const QVariant &value);

    virtual void getData(QVariant &value);

private:
    TempModuleData              *m_pTempModuleData;
    modulepackets           m_modulePacket;
};

Q_DECLARE_METATYPE(TempModuleData *)

#endif // MODULETEMP_H
