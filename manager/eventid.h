#ifndef EVENTID_H
#define EVENTID_H

#include <QEvent>

enum
{
    EV_Start = QEvent::User,

    EV_Init,
    EV_Quit,

    EV_AdmitPatient,
    EV_DischargePatient,
    EV_UpdatePatient,
    EV_UpdateStatusBar,

    EV_CreateDatabase,
    EV_DischargeDatabase,

    EV_SaveAlarmData,
    EV_SaveNibpData,                	// 用于通知数据库保存ｎｉｂｐ数据和ｎｉｂｐ列表更新数据
    EV_SaveCOData,
    EV_SaveEWSData,
    EV_AutoSaveEWSData,
    EV_SaveGCSData,

    EV_SaveCustomEventData,

    EV_CollectClinicData,               // 通知保存原始的临床数据

    EV_UpdateEventData,
    EV_RemoveEventData,

    EV_RequestTrendData,
    EV_ResponseTrendData,

    EV_RequestTrendWaveData,
    EV_ResponseTrendWaveData,

    EV_RequestWavesData,
    EV_ResponseWavesData,

    EV_RequestAlarmData,
    EV_ResponseAlarmData,

    EV_RequestAlarmWavesData,
    EV_ResponseAlarmWavesData,

    EV_RequestNibpData,
    EV_ResponseNibpData,

    EV_RequestEWSData,
    EV_ResponseEWSData,

    EV_RequestGCSData,
    EV_ResponseGCSData,

    EV_RequestOxygenData,
    EV_ResponseOxygenData,

    EV_RequestDateTimeLimit,
    EV_ResponseDateTimeLimit,

    EV_RequestSystemData,
    EV_ResponseSystemData,

    EV_RequestCOData,
    EV_ResponseCOData,

    EV_RequestCustomEventData,
    EV_ResponseCustomEventData,

    EV_RequestCustomEventWaveData,
    EV_ResponseCustomEventWaveData,

    EV_UpdateSystemTime,

    EV_ModuleOnline,
    EV_ModuleOffline,

    EV_PausePaintWave,
    EV_FreezeWave,

    EV_SendTecAlarmData,
    EV_SendPhyAlarmData,

    EV_StartRecordOnce,
    EV_StartRealTimeRecord,
    EV_StopRealTimeRecord,
    EV_UpDataCycleRecordInterval,
    EV_StopAllRecordTask,

    EV_UpdateScreenLayout,                  // 界面重新布局事件(模块计算导联，改变时发送事件，重新布局)

    EV_SetSysConfig,  						// 主要用于报警模块更新配置
    EV_UpdateShortCutKeyLayout,				// 更新快捷键的布局

    EV_TriggerSystemEvent,                  // 请求/触发系统事件动作
    EV_ResponseTriggerSystemEvent,          // 回复系统事件动作
    EV_SetStandbyMode,                      // 进入standby的原因

    EV_MainboardRequestAction,              // 主板请求事件
    EV_SingleStateNotification,             // 单次事件通知,比如进入了报警暂停,报警暂停时间结束了等等

    EV_SendPrintInfoToNet,                  // 发送打印内容到网络
    EV_ResponePrintInfoToNet,               // 应答打印内容到网络

    EV_MAX
};

#endif // EVENTID_H
