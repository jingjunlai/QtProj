#include "moduleco2.h"
#include "mathtools.h"
#include "net/cmsco2moduledata.h"
#include <QVariant>
#include <QDebug>
#include <QThread>

CO2Param::CO2Param(ModuleID module, LabelID label)
    : ModuleParam(module, label)
{
}

CO2Param::~CO2Param()
{

}

QList<ParamPoint> CO2Param::toList()
{
    QList<ParamPoint> co2Param;

    ParamPoint point;
    point.label = LABEL_CO2;
    point.subid = Sub_CO2Et;
    point.data = ExHalePot;
    co2Param.append(point);

    point.label = LABEL_CO2;
    point.subid = Sub_CO2Fi;
    point.data = InHalePot;
    co2Param.append(point);

    return co2Param;
}

//****************CO2Status class*******************//
CO2Status::CO2Status(ModuleID module, LabelID label)
    : ModuleStatus(module, label)
{

}

void CO2Status::reset()
{
    nackError = 0 ;             //NACK ERROR
    haveConnectedBody = 0;     //0 没有连接过人体 1 连接过人体
    apnea = 0;                 //窒息状态
    hardwareState = 0;         //硬件状态
    priorityStatus = 0;            //优先级异常状态
    noBreathDetected = 0;      //呼吸标志 1为未检测到呼吸 1未检测到
    inSleepMode = 0;           //1:睡眠模式  0:工作模式
    notReadyZero = 0;          //是否可以校零 1表示未准备好校零
    co2OutOfRange = 0;         //CO2超范围
    breathDetected = 0;        //呼吸标志 1为检测到呼吸 0未检测到
    checkAdapter = 0;          //检查适配器
    negativeCo2Error = 0;      //负CO2错误
    sourceCurrentStatus = 0;   //电流状态 0 正常 1 等待电流稳定 2 源电流不稳定 3 源电流限制错误  3和4 属于硬件错误
    compensationNotYetSet = 0; //未初始化
    calibrationStatus = 0;     //校零状态过程 0 无校零正在进行 1正在校零 2 需要校零 3校零错误
    temperatureStatus = 0;     //传感器加热状态
    eepromError = 0;           //EEPROM checksum faulty
    hardwareError = 0;         //硬件错误
    pumpOff = 0;               //泵关闭
    checkSampleLine = 0;       //检查取样线
    pumpLifeExceeded = 0;      //泵超过使用寿命
    sampleLineDisconnected = 0;

    sensorError = 0;           //bit0=1:softwave error   bit1=1:hardwave error  bit2=1:motor error  bit3=1:factory error
    adapterStatus = 0;         //BIT0 = 1:(MIAN:replace adapter Side:Sampling line clogged)
    dataValid = 0;             //详见手册 注：BIT6=1, zero required!
    nobreathsTimeout = 20;     // 20 - 60s
    sensorCfgRegister0 = 0;
    sensorCfgRegister1 = 0;
    sensorHardwareRevision = 0;
    sensorSoftwareRevision = 0; //byte 2 =hi byte3 =lo
    streamModel = Co2_Sidestream;      //0旁流还是1主流
    commProtocolRevision = 0;
    sensorSNNumber = 0;              //byte0:mid byte byte1:low byte byte4 hi byte
    serviceStatusRegister = 0;
}

//**************************CO2Wave******************************//
CO2Wave::CO2Wave(int size, ModuleID module, LabelID label)
    : ModuleWave(size, module, label)
{
}

CO2Wave::~CO2Wave()
{
//    if(buffer)
    //        delete [] buffer;
}

QList<WavePoint> CO2Wave::toList(WaveID id)
{
    QList<WavePoint> co2WavePoint;
    if (id != WAVE_CO2)
        return co2WavePoint;

    for (int i = 0; i < size(); i++)
    {
        WavePoint point;
        point.data = (data() + i)->waveValue;
        point.valid = true;
        co2WavePoint.append(point);
    }

    return co2WavePoint;
}

Co2ModuleData::Co2ModuleData(ModuleID module, LabelID label)
    : ModuleData(module, label)
{
    m_pParam = new CO2Param(module, label);
    m_pStatus = new CO2Status(module, label);
    m_pWave = new RingBuffer<CO2WaveUnit>(100 * 3);
    reset();
}

Co2ModuleData::~Co2ModuleData()
{
    delete m_pParam;
    delete m_pStatus;
    delete m_pWave;
}

void Co2ModuleData::reset()
{
    m_pParam->reset();
    m_pStatus->reset();
}

ModuleCo2::ModuleCo2(ModuleProperty property, QObject *parent)
    : IModule(property, parent)
{
    m_pCo2ModuleData = new Co2ModuleData(property.moduleId, property.labelId);
}

ModuleCo2::~ModuleCo2()
{
    delete m_pCo2ModuleData;
}

void ModuleCo2::unpackModuleData(const QByteArray &packet)
{
    Q_UNUSED(packet);
}

bool ModuleCo2::setConfig(int id, const QVariant &value)
{
    Q_UNUSED(id);
    Q_UNUSED(value);
    return false;
}

bool ModuleCo2::getConfig(int id, QVariant &value)
{
    Q_UNUSED(id);
    Q_UNUSED(value);
    return false;
}

void ModuleCo2::setData(const QVariant &value)
{
    auto d = value.value<Co2DataField>();

    m_pCo2ModuleData->m_pStatus->sensorOff = !d.working;

    if(d.fiValid == true)
        m_pCo2ModuleData->m_pParam->InHalePot = d.fiValue;
    else
        m_pCo2ModuleData->m_pParam->InHalePot = INVALID_DATA;

    if(d.etValid == true)
        m_pCo2ModuleData->m_pParam->ExHalePot = d.etValue;
    else
        m_pCo2ModuleData->m_pParam->ExHalePot = INVALID_DATA;

    if(d.awrrValid == true)
        m_pCo2ModuleData->m_pParam->breathRate = d.awrrValue;
    else
        m_pCo2ModuleData->m_pParam->breathRate = INVALID_DATA;

    m_modulePacket.ParamPackets++;
    m_modulePacket.WavePackets += d.waves.size();
    if(m_modulePacket.Count++ >= 1)
    {
        m_modulePacket.Count = 0;
        m_pCo2ModuleData->m_ModuleInfo.packets.ParamPackets = m_modulePacket.ParamPackets;
        m_pCo2ModuleData->m_ModuleInfo.packets.WavePackets = m_modulePacket.WavePackets;
        m_modulePacket.ParamPackets = 0;
        m_modulePacket.WavePackets = 0;
    }

    CO2WaveUnit *pw = new CO2WaveUnit[d.waves.size()];

    for(int i = 0; i < d.waves.size(); i++)
    {
        auto FrameData = d.waves.at(i);

        pw[i].bVaild = d.working;
        pw[i].waveValue = FrameData.co2Wave;
    }
    if(d.waves.size() <= m_pCo2ModuleData->m_pWave->capacity())
    {
        m_pCo2ModuleData->m_pWave->write(pw, d.waves.size());
    }
    else
    {
        qDebug() << "Co2 too much Data!!";
    }
    delete [] pw;
//    qDebug() << "module co2 set Data";
}

void ModuleCo2::getData(QVariant &value)
{
    value.setValue(m_pCo2ModuleData);
}
