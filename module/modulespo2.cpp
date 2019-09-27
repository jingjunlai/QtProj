#include "modulespo2.h"
#include "mathtools.h"
#include "net/cmsspo2moduledata.h"
#include <QVariant>
#include <QDebug>
#include <QThread>

Spo2Param::Spo2Param(ModuleID module, LabelID label)
    : ModuleParam(module, label)
{

}

Spo2Param::~Spo2Param()
{

}

void Spo2Param::reset()
{
     m_iConnectState = 0;         //判断是否连接过人体，处理报警的 时候是 不同的
     m_iPrValue = INVALID_DATA;  //从血氧模块来的PR ,  脉率的有效测量范围是0-300，511 = 无效数据
     m_iSpO2Value = INVALID_DATA;// 血氧值,   血氧值的测量范围是0-100；>100(255)表示无效数据
     m_iPIValue = INVALID_DATA;  //PI值, 扩大1000倍上传，有效范围为0~20.000%       建议显示至小数点后2位
     m_iPRUpdateTimeout = 1;      //1=脉率数值没有更新已超过25秒 0=脉率数值有更新
     m_iSpO2UpdateTimeout = 1;    //1=血氧数值没有更新已超过25秒 0=血氧数值有更新
     m_iIsUpdatePR = 1;
     m_iIsUpdateSpO2 = 1;
     m_iBloodColumnStrength = 0;  // 血柱强度（0-15）
     m_iSignalQualityIndex = 0;   //信号质量指数    范围为0~100     单位为%，低于60%应交替显示“?”
}

QList<ParamPoint> Spo2Param::toList()
{
    QList<ParamPoint> ret;

    ParamPoint point;
    point.label = labelId();
    point.subid = Sub_Spo2;
    point.data = m_iSpO2Value;
    ret.append(point);

    return ret;
}

Spo2Status::Spo2Status(ModuleID module, LabelID label)
    : ModuleStatus(module, label)
{

}

Spo2Status::~Spo2Status()
{

}

void Spo2Status::reset()
{
//    moduleState = Module_StateOnline;              //0x00=Online状态  0x01=Fault错误状态 0x02= 模块被禁止 0x03=HandUp状态
    m_iSearchTooLong = 0;            //1 =脉搏信号弱（脉搏搜索时间太长）0 = 正常 ;Searching too long应为高级生理报警
    m_iSearchingPulse = 0;           //1 = 正在搜索脉搏, Searching pulse，提示正在搜索脉搏
    m_iMotionInterference = 0;       //1 = 有运动干扰          提示信息
    m_iAverageTime = 0;              //平均时间：00：正常(约8秒) 01：快(约4秒) 02：慢(约16秒)
    m_iSensitivityMode = 0;          //灵敏级别 灵敏度：0：普通，1高
    m_iSensorOff = 0;
    m_iProbeLose = 0;
//    errorCode = 0;                  //错误代码 0：模块工作正常  其它:表示模块存在问题，上传模块错误代码。当此字节不为0时，所有参数和波形均无效
}

Spo2Wave::Spo2Wave(int size, ModuleID module, LabelID label)
    : ModuleWave(size, module, label)
{
}

Spo2Wave::~Spo2Wave()
{
}

QList<WavePoint> Spo2Wave::toList(WaveID wave)
{
    QList<WavePoint> ret;
    if (wave != WAVE_SPO2)
        return ret;
    for (int i = 0; i < size(); i++)
    {
        WavePoint point;
        point.data = (data() + i)->wave;
        point.valid = (data() + i)->bValid;
        ret.append(point);
    }
    return ret;

}

Spo2ModuleData::Spo2ModuleData(ModuleID module, LabelID label)
    : ModuleData(module, label)
{
    m_pParam = new Spo2Param(module, label);
    m_pStatus = new Spo2Status(module, label);
    m_pWave = new RingBuffer<Spo2WaveUnit>(100 * 6);
    reset();
}

Spo2ModuleData::~Spo2ModuleData()
{
    delete m_pParam;
    delete m_pStatus;
    delete m_pWave;
}

void Spo2ModuleData::reset()
{
    m_pParam->reset();
    m_pStatus->reset();
}

ModuleSpo2::ModuleSpo2(ModuleProperty property, QObject *parent)
    : IModule(property, parent)
{
    m_pSpo2ModuleData = new Spo2ModuleData(property.moduleId, property.labelId);
}

ModuleSpo2::~ModuleSpo2()
{
    delete m_pSpo2ModuleData;
}

void ModuleSpo2::unpackModuleData(const QByteArray &packet)
{
    Q_UNUSED(packet);
}

bool ModuleSpo2::setConfig(int id, const QVariant &value)
{
    Q_UNUSED(id);
    Q_UNUSED(value);
    return false;
}

bool ModuleSpo2::getConfig(int id, QVariant &value)
{
    Q_UNUSED(id);
    Q_UNUSED(value);
    return false;
}

void ModuleSpo2::setData(const QVariant &value)
{
    auto d = value.value<Spo2DataField>();

    m_pSpo2ModuleData->m_pParam->m_iSpO2Value = (d.spo2 >= 255? INVALID_DATA : d.spo2);
    m_pSpo2ModuleData->m_pParam->m_iPIValue = (d.pi == INVALID_DATA?  INVALID_DATA : d.pi);
    m_pSpo2ModuleData->m_pParam->m_iPrValue = (d.pulseRate > 300? INVALID_DATA: d.pulseRate);

    m_pSpo2ModuleData->m_pStatus->m_iSensorOff = d.sensorOff;
    m_pSpo2ModuleData->m_pStatus->m_iProbeLose = d.cableOff;
    m_pSpo2ModuleData->m_pStatus->m_iSearchTooLong = d.weakPulse;
    m_pSpo2ModuleData->m_pStatus->m_iSearchingPulse = d.searching;
    m_pSpo2ModuleData->m_pStatus->m_iMotionInterference = d.interference;

    m_modulePacket.ParamPackets++;
    m_modulePacket.WavePackets += d.waves.size();
    if(m_modulePacket.Count++ >= 1)
    {
        m_modulePacket.Count = 0;
        m_pSpo2ModuleData->m_ModuleInfo.packets.ParamPackets = m_modulePacket.ParamPackets;
        m_pSpo2ModuleData->m_ModuleInfo.packets.WavePackets = m_modulePacket.WavePackets;
        m_modulePacket.ParamPackets = 0;
        m_modulePacket.WavePackets = 0;
    }

    Spo2WaveUnit *pw = new Spo2WaveUnit[d.waves.size()];

    for(int i = 0; i < d.waves.size(); i++)
    {
        auto FrameData = d.waves.at(i);
        if((pw[i].bValid = ((d.sensorOff || d.cableOff)? false : true)) == false)
            continue;
        pw[i].rFlag = FrameData.rwave;
        pw[i].wave = FrameData.spo2Wave;
        pw[i].respwave = FrameData.respWave;
    }
    if(d.waves.size() <= m_pSpo2ModuleData->m_pWave->capacity())
    {
        m_pSpo2ModuleData->m_pWave->write(pw, d.waves.size());
    }
    else
    {
        qDebug() << "Spo2 too much Data!!";
    }
    delete [] pw;

//    qDebug() << "module spo2 set Data = " << value;
}

void ModuleSpo2::getData(QVariant &value)
{
    value.setValue(m_pSpo2ModuleData);
}

