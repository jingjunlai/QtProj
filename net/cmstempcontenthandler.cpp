#include "cmstempcontenthandler.h"
#include "cmstempmoduledata.h"
#include "cmstempconfigdata.h"
#include "module/moduledata.h"
#include "module/valueid.h"
#include <QDebug>
#include <QVariant>

CmsTempContentHandler::CmsTempContentHandler(LabelID label, QObject *parent)
    : CmsContentHandler(label, parent)
{

}

CmsTempContentHandler::~CmsTempContentHandler()
{

}

bool CmsTempContentHandler::handleModuleData(const QByteArray &arr)
{
    //qDebug() << "temp moudule data = " << arr.toHex();

    CmsModuleID identifier = (CmsModuleID)arr.at(0);
    CmsLabelID label = (CmsLabelID)arr.at(1);

    auto data = new CmsTempModuleData(identifier, label);
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
    auto d = ((CmsTempModuleData*)data)->data();
    delete data;

    /*发送给model*/
    QVariant *pv = new QVariant;
    pv->setValue(d);

    emit sendDataField(mLabelId, pv);

//    ParamPoint param;
//    param.label = mLabelId;
//    param.subid = d.temp[0].subid;
//    param.data = d.temp[0].data;
//    mModuleData.params.append(param);

//    param.subid = d.temp[1].subid;
//    param.data = d.temp[1].data;
//    mModuleData.params.append(param);

    // temp hasn't wave

    return true;
}

bool CmsTempContentHandler::handleConfigData(const QByteArray &arr)
{

    //qDebug() << "temp config data = " << arr.toHex();

    if (arr.size() < 2)
        return false;

    CmsModuleID identifier = (CmsModuleID)arr.at(0);
    CmsLabelID label = (CmsLabelID)arr.at(1);
    auto conf = new CmsTempConfigData(identifier, label);
    if (conf == Q_NULLPTR)
        return false;

    if (!conf->fromByteArray(arr))
        return false;

    auto d = conf->config();
    delete conf;

    Q_UNUSED(d);
    // save
    // temp1
//    {
//        QVariant value;
//        value = QVariant(d.temp[0].alarmSetting.sw ? Gps::On : Gps::Off);
//        moduleManager->setModuleConfig(mLabelId, TC_Temp1AlarmNormalSwitch, value);

//        int level = qMin(CMS_AlarmHighLevel, d.temp[0].alarmSetting.level);
//        value = QVariant(level);
//        moduleManager->setModuleConfig(mLabelId, TC_Temp1AlarmNormalLevel, value);

//        value = QVariant(d.temp[0].alarmSetting.limit[CmsAlarmConfig<>::up_index]);
//        moduleManager->setModuleConfig(mLabelId, TC_Temp1AlarmNormalHighLimit, value);

//        value = QVariant(d.temp[0].alarmSetting.limit[CmsAlarmConfig<>::down_index]);
//        moduleManager->setModuleConfig(mLabelId, TC_Temp1AlarmNormalLowLimit, value);
//    }

//    // temp2
//    {
//        QVariant value;
//        value = QVariant(d.temp[1].alarmSetting.sw ? Gps::On : Gps::Off);
//        moduleManager->setModuleConfig(mLabelId, TC_Temp2AlarmNormalSwitch, value);

//        int level = qMin(CMS_AlarmHighLevel, d.temp[1].alarmSetting.level);
//        value = QVariant(level);
//        moduleManager->setModuleConfig(mLabelId, TC_Temp2AlarmNormalLevel, value);

//        value = QVariant(d.temp[1].alarmSetting.limit[CmsAlarmConfig<>::up_index]);
//        moduleManager->setModuleConfig(mLabelId, TC_Temp2AlarmNormalHighLimit, value);

//        value = QVariant(d.temp[1].alarmSetting.limit[CmsAlarmConfig<>::down_index]);
//        moduleManager->setModuleConfig(mLabelId, TC_Temp2AlarmNormalLowLimit, value);
//    }

//    // delta T
//    {
//        QVariant value;
//        value = QVariant(d.deltaTAlarmSetting.sw ? Gps::On : Gps::Off);
//        moduleManager->setModuleConfig(mLabelId, TC_TdAlarmNormalSwitch, value);

//        int level = qMin(CMS_AlarmHighLevel, d.deltaTAlarmSetting.level);
//        value = QVariant(level);
//        moduleManager->setModuleConfig(mLabelId, TC_TdAlarmNormalLevel, value);

//        value = QVariant(d.deltaTAlarmSetting.limit[CmsAlarmConfig<>::up_index]);
//        moduleManager->setModuleConfig(mLabelId, TC_TdAlarmNormalHighLimit, value);

//        // 没有下限
//    }

    return true;
}


