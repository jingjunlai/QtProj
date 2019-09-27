#ifndef CMSTYPE
#define CMSTYPE

#include "module/valueid.h"

#include <QByteArray>
#include <QList>

#define PORT_FOR_CMS                9100
#define PORT_FOR_OTHERBED_SERVER    9200
#define PORT_FOR_BROADCAST          9001

#define CMS_VERSION_MAJOR           0
#define CMS_VERSION_MINOR           1

typedef unsigned char Byte;

typedef enum
{
    WR_CentralMonitorSystem,
    WR_BedMonitorServer,
    WR_BedMonitorClient,
} WorkRole;

typedef enum
{
    CMS_MessageUnkown = 0x00,

    CMS_MessageUpHeartBeat = 0xd0,
    CMS_MessageUpModuleData = 0xd1,
    CMS_MessageUpConfig = 0xd2,
    CMS_MessageUpAlarmInfo = 0xd3,
    CMS_MessageUpArrInfo = 0xd4,
    CMS_MessageUpStTemplate = 0xd6,
    CMS_MessageUpCommand = 0xdc,
    CMS_MessageUpHistoryData = 0xdf,

    CMS_MessageDownHeartBeat = 0xd0,
    CMS_MessageDownConfig = 0xd5,
    CMS_MessageDownCommand = 0xdc,

    CMS_MessageResponse = 0xda,

    CMS_MessageBroadcast = 0xdb,
} CmsMessageType;

typedef enum
{
    CMS_CommandUnknown = 0x00,

    CMS_CommandUpPatient = 0x81,
    CMS_CommandUpRequestBedList = 0x82,
    CMS_CommandUpRequestHistoryData = 0x83,

    CMS_CommandDownStartNibp = 0x01,
    CMS_CommandDownStopNibp = 0x02,
    CMS_CommandDownAdmitDischargePatient = 0x03,
    CMS_CommandDownSyncTime = 0x04,
    CMS_CommandDownContextualModel = 0x05,

} CmsCommandType;

typedef enum
{
    CMS_LabelNull,
    CMS_LabelEcg = 1,
    CMS_LabelSpo2 = 2,
    CMS_LabelNibp = 3,
    CMS_LabelResp = 4,
    CMS_LabelCo2 = 5,
    CMS_LabelIcg = 6,
    CMS_LabelBis = 7,
    CMS_LabelAa = 8,
    CMS_LabelO2 = 9,
    CMS_LabelN2o = 10,
    CMS_LabelPr = 11,
    CMS_LabelCo = 14,
    CMS_LabelCsm = 15,
    CMS_LabelIbpP1 = 20,
    CMS_LabelIbpP2 = 21,
    CMS_LabelIbpArt = 22,
    CMS_LabelIbpCvp = 23,
    CMS_LabelIbpPa = 24,
    CMS_LabelIbpRap = 25,
    CMS_LabelIbpLap = 26,
    CMS_LabelIbpIcp = 27,
    CMS_LabelIbpPawp = 28,
    CMS_LabelIbpUap= 29,
    CMS_LabelIbpBap = 30,
    CMS_LabelIbpFap = 31,
    CMS_LabelIbpUvp = 32,
    CMS_LabelIbpLv = 33,
    CMS_LabelIbpAo = 34,
    CMS_LabelTof = 36,
    CMS_LabelDm = 40,
    CMS_LabelTempTA = 50,
    CMS_LabelTempTB = 51,
    CMS_LabelTempTC = 52,
    CMS_LabelTempTD = 53,
    CMS_LabelTempTE = 54,
//    CMS_LabelArr = 253, //增加一个ARR BY LJJ 2019.6.10
    CMS_LabelMachine = 254, // 0xfe
} CmsLabelID;

typedef enum
{
    CMS_ModuleNull = 0x00,
    CMS_ModuleEcg = 0xe0,
    CMS_ModuleSpo2 = 0xe1,
    CMS_ModuleResp = 0xe3,
    CMS_ModuleNibp = 0xe5,
    CMS_ModuleIbp = 0xe6,
    CMS_ModuleTemp = 0xe4,
    CMS_ModuleCo2 = 0xe7,
    CMS_ModuleO2 = 0xe8,
    CMS_ModuleN2o = 0xe9,
    CMS_ModuleAa = 0xea,
    CMS_ModuleIcg = 0xeb,
    CMS_ModuleBis = 0xf0,
    CMS_ModuleCsm = 0xf1,
    CMS_ModuleCo = 0xf2,
    CMS_ModuleEeg = 0xf3,
    CMS_ModuleRm = 0xf4,
    CMS_ModuleDm = 0xf5,
    CMS_ModuleMachine = 0xfe,
} CmsModuleID;

typedef enum
{
    CMS_ProbeStateNormal = 0,                  //探头正常
    CMS_ProbeStateIncompatible,                //不兼容
    CMS_ProbeStateUnActivated,                 //未激活
    CMS_ProbeStateOverdued,                    //已过期
    CMS_ProbeStateBeingOutOfDate,              //即将过期
    CMS_ProbeStateMax
} CmsProbeState;

struct InvalidData
{
    static const int nibp = 0xffff;
};

typedef enum
{
    CMS_AlarmTipLevel,
    CMS_AlarmLowLevel,
    CMS_AlarmMidLevel,
    CMS_AlarmHighLevel,
} CmsAlarmLevel;

typedef enum
{
    CMS_AdmitPatient,
    CMS_UpdatePatient,
    CMS_DischargePatient,
} CmsPatHandleType;

typedef enum
{
    SENDER_Running,
    SENDER_Standby,
    SENDER_Shutdown,
    SENDER_EmsPullOut,
} SenderRunninigStatus;

struct CmsActivationTime
{
    unsigned char year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
};
#define PROBE_IDENTIFIER_LENGTH 8       // 探头唯一识别码的长度8个ascci字符
struct CmsProbeInfo
{
    unsigned char       probeId[PROBE_IDENTIFIER_LENGTH];       // 探头唯一标识
    CmsActivationTime   activationTime;                         // 探头激活时间
    unsigned short      useTimes;                               // 探头使用次数
    unsigned int        serviceTime;                            // 探头使用时长/服务时间长度
};

struct CmsConnectionSettings
{
    QString host()
    {
        return !hostName.isEmpty() ? hostName : ipAddress;
    }

    QString     hostName;
    QString     ipAddress;
    int         port = 0;
    int         timeoutms = 2000;
};

const int INVALID_MACHINE_NO = 255;

struct MachineNo
{
    MachineNo(unsigned char no = INVALID_MACHINE_NO)
    {
        this->no = no;
        if (!isValid())
            this->no = INVALID_MACHINE_NO;
    }

    MachineNo &operator =(unsigned char no)
    {
        this->no = no;
        return *this;
    }

    bool operator == (int n) const
    {
        return *this == MachineNo(n);
    }

    bool operator == (const MachineNo &other) const
    {
        return this->no == other.no;
    }

    bool operator != (const MachineNo &other) const
    {
        return !(operator == (other));
    }

    bool operator < (const MachineNo &other) const
    {
        return this->no < other.no;
    }

    bool isValid() const
    {
        return no >= Gps::OTHERBED_MIN_NO && no <= Gps::OTHERBED_MAX_NO;
    }

    unsigned char no;
};

Q_DECLARE_METATYPE(MachineNo)

#endif // CMSTYPE

