#include "cmsnibpcontenthandler.h"
#include "cmsnibpmoduledata.h"
#include "cmsnibpconfigdata.h"
#include "module/moduledata.h"
#include "module/valueid.h"
#include <QDebug>
#include <QVariant>

CmsNibpContentHandler::CmsNibpContentHandler(LabelID label, QObject *parent)
    : CmsContentHandler(label, parent)
{

}

CmsNibpContentHandler::~CmsNibpContentHandler()
{

}

bool CmsNibpContentHandler::handleModuleData(const QByteArray &arr)
{
    //qDebug() << "nibp moudule data = " << arr.toHex();

    CmsModuleID identifier = (CmsModuleID)arr.at(0);
    CmsLabelID label = (CmsLabelID)arr.at(1);

    auto data = new CmsNibpModuleData(identifier, label);
    if (data == Q_NULLPTR)
        return false;

    /*解包，并放在dataField里*/
    if (!data->fromByteArray(arr))
    {
        delete data;
        qDebug() << "ecg handle from byte error!";
        return false;
    }
    /*读出dataField*/
    auto d = ((CmsNibpModuleData*)data)->data();
    delete data;

    /*发送给model*/
    QVariant *pv = new QVariant;
    pv->setValue(d);

    emit sendDataField(mLabelId, pv);

//    ParamPoint param;
//    param.label = mLabelId;
//    param.subid = Sub_NibpSys;
//    param.data = d.sys;
//    mModuleData.params.append(param);

//    param.subid = Sub_NibpDia;
//    param.data = d.dia;
//    mModuleData.params.append(param);

//    param.subid = Sub_NibpMap;
//    param.data = d.map;
//    mModuleData.params.append(param);

    // nibp hasn't wave

    return true;
}

bool CmsNibpContentHandler::handleConfigData(const QByteArray &arr)
{
    if (arr.size() < 2)
        return false;

    CmsModuleID identifier = (CmsModuleID)arr.at(0);
    CmsLabelID label = (CmsLabelID)arr.at(1);
    auto conf = new CmsNibpConfigData(identifier, label);
    if (conf == Q_NULLPTR)
        return false;

    if (!conf->fromByteArray(arr))
        return false;

    auto d = conf->config();
    delete conf;

    // save
    // sys alarm
    {
        QVariant value;
        value = QVariant(d.sysAlarmSetting.sw ? Gps::On : Gps::Off);
        //moduleManager->setModuleConfig(mLabelId, NIBPC_SysAlarmNormalSwitch, value);

        int level = qMin(CMS_AlarmHighLevel, d.sysAlarmSetting.level);
        value = QVariant(level);
        //moduleManager->setModuleConfig(mLabelId, NIBPC_SysAlarmNormalLevel, value);

        value = QVariant(d.sysAlarmSetting.limit[CmsAlarmConfig<>::up_index]);
        //moduleManager->setModuleConfig(mLabelId, NIBPC_SysAlarmNormalHighLimit, value);

        value = QVariant(d.sysAlarmSetting.limit[CmsAlarmConfig<>::down_index]);
        //moduleManager->setModuleConfig(mLabelId, NIBPC_SysAlarmNormalLowLimit, value);
    }

    // dia alarm
    {
        QVariant value;
        value = QVariant(d.diaAlarmSetting.sw ? Gps::On : Gps::Off);
        //moduleManager->setModuleConfig(mLabelId, NIBPC_DiaAlarmNormalSwitch, value);

        int level = qMin(CMS_AlarmHighLevel, d.diaAlarmSetting.level);
        value = QVariant(level);
        //moduleManager->setModuleConfig(mLabelId, NIBPC_DiaAlarmNormalLevel, value);

        value = QVariant(d.diaAlarmSetting.limit[CmsAlarmConfig<>::up_index]);
        //moduleManager->setModuleConfig(mLabelId, NIBPC_DiaAlarmNormalHighLimit, value);

        value = QVariant(d.diaAlarmSetting.limit[CmsAlarmConfig<>::down_index]);
        //moduleManager->setModuleConfig(mLabelId, NIBPC_DiaAlarmNormalLowLimit, value);
    }

    // mean alarm
    {
        QVariant value;
        value = QVariant(d.meanAlarmSetting.sw ? Gps::On : Gps::Off);
        //moduleManager->setModuleConfig(mLabelId, NIBPC_MapAlarmNormalSwitch, value);

        int level = qMin(CMS_AlarmHighLevel, d.meanAlarmSetting.level);
        value = QVariant(level);
        //moduleManager->setModuleConfig(mLabelId, NIBPC_MapAlarmNormalLevel, value);

        value = QVariant(d.meanAlarmSetting.limit[CmsAlarmConfig<>::up_index]);
        //moduleManager->setModuleConfig(mLabelId, NIBPC_MapAlarmNormalHighLimit, value);

        value = QVariant(d.meanAlarmSetting.limit[CmsAlarmConfig<>::down_index]);
        //moduleManager->setModuleConfig(mLabelId, NIBPC_MapAlarmNormalLowLimit, value);
    }

    // sdp alarm
    {
        QVariant value;
        value = QVariant(d.sdpAlarmSetting.sw ? Gps::On : Gps::Off);
        //moduleManager->setModuleConfig(mLabelId, NIBPC_SdpAlarmNormalSwitch, value);

        int level = qMin(CMS_AlarmHighLevel, d.sdpAlarmSetting.level);
        value = QVariant(level);
        //moduleManager->setModuleConfig(mLabelId, NIBPC_SdpAlarmNormalLevel, value);

        value = QVariant(d.sdpAlarmSetting.limit[CmsAlarmConfig<>::up_index]);
        //moduleManager->setModuleConfig(mLabelId, NIBPC_SdpAlarmNormalHighLimit, value);

        value = QVariant(d.sdpAlarmSetting.limit[CmsAlarmConfig<>::down_index]);
        //moduleManager->setModuleConfig(mLabelId, NIBPC_SdpAlarmNormalLowLimit, value);
    }

    return true;
}


