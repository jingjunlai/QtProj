#include "cmsibpcontenthandler.h"
#include "cmsibpmoduledata.h"
#include "cmsibpconfigdata.h"
#include "module/moduledata.h"
#include "module/valueid.h"
#include <QDebug>
#include <QVariant>

CmsIbpContentHandler::CmsIbpContentHandler(LabelID label, QObject *parent)
    : CmsContentHandler(label, parent)
{

}

CmsIbpContentHandler::~CmsIbpContentHandler()
{

}

bool CmsIbpContentHandler::handleModuleData(const QByteArray &arr)
{
    //qDebug() << "ecg moudule data = " << arr.toHex();

    CmsModuleID identifier = (CmsModuleID)arr.at(0);
    CmsLabelID label = (CmsLabelID)arr.at(1);

    auto data = new CmsIbpModuleData(identifier, label);
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
    auto d = ((CmsIbpModuleData*)data)->data();
    delete data;

    /*发送给model*/
    QVariant *pv = new QVariant;
    pv->setValue(d);

    emit sendDataField(mLabelId, pv);

//    ParamPoint param;
//    param.label = mLabelId;
//    param.subid = Sub_IbpSys;
//    param.data = d.sys;
//    mModuleData.params.append(param);

//    param.subid = Sub_IbpDia;
//    param.data = d.dia;
//    mModuleData.params.append(param);

//    param.subid = Sub_IbpMean;
//    param.data = d.mean;
//    mModuleData.params.append(param);

//    QList<WavePoint> wave;
//    for (int i = 0; i < d.waves.size(); i++)
//    {
//        auto frameData = d.waves.at(i);
//        WavePoint tmp;
//        tmp.data = frameData.ibpWave;
//        wave.append(tmp);
//    }

//    WaveID id = (WaveID)(WAVE_IBP_START + (mLabelId - LABEL_IBP_START));
//    mModuleData.waves.insert(id, wave);

    return true;
}

bool CmsIbpContentHandler::handleConfigData(const QByteArray &arr)
{
    if (arr.size() < 2)
        return false;

    CmsModuleID identifier = (CmsModuleID)arr.at(0);
    CmsLabelID label = (CmsLabelID)arr.at(1);
    auto conf = new CmsIbpConfigData(identifier, label);
    if (conf == Q_NULLPTR)
        return false;

    if (!conf->fromByteArray(arr))
        return false;

    auto d = conf->config();
    delete conf;

    Q_UNUSED(d);

    // save
    // sys alarm
//    {
//        QVariant value;
//        value = QVariant(d.sysAlarmSetting.sw ? Gps::On : Gps::Off);
//        moduleManager->setModuleConfig(mLabelId, IBPC_SysAlarmNormalSwitch, value);
//        qDebug() << value;

//        int level = qMin(CMS_AlarmHighLevel, d.sysAlarmSetting.level);
//        value = QVariant(level);
//        moduleManager->setModuleConfig(mLabelId, IBPC_SysAlarmNormalLevel, value);
//        qDebug() << value;

//        value = QVariant(d.sysAlarmSetting.limit[CmsAlarmConfig<>::up_index]);
//        moduleManager->setModuleConfig(mLabelId, IBPC_SysAlarmNormalHighLimit, value);
//        qDebug() << value;

//        value = QVariant(d.sysAlarmSetting.limit[CmsAlarmConfig<>::down_index]);
//        moduleManager->setModuleConfig(mLabelId, IBPC_SysAlarmNormalLowLimit, value);
//        qDebug() << value;
//    }

//    // dia alarm
//    {
//        QVariant value;
//        value = QVariant(d.diaAlarmSetting.sw ? Gps::On : Gps::Off);
//        moduleManager->setModuleConfig(mLabelId, IBPC_DiaAlarmNormalSwitch, value);

//        int level = qMin(CMS_AlarmHighLevel, d.diaAlarmSetting.level);
//        value = QVariant(level);
//        moduleManager->setModuleConfig(mLabelId, IBPC_DiaAlarmNormalLevel, value);

//        value = QVariant(d.diaAlarmSetting.limit[CmsAlarmConfig<>::up_index]);
//        moduleManager->setModuleConfig(mLabelId, IBPC_DiaAlarmNormalHighLimit, value);

//        value = QVariant(d.diaAlarmSetting.limit[CmsAlarmConfig<>::down_index]);
//        moduleManager->setModuleConfig(mLabelId, IBPC_DiaAlarmNormalLowLimit, value);
//    }

//    // mean alarm
//    {
//        QVariant value;
//        value = QVariant(d.meanAlarmSetting.sw ? Gps::On : Gps::Off);
//        moduleManager->setModuleConfig(mLabelId, IBPC_MeanAlarmNormalSwitch, value);

//        int level = qMin(CMS_AlarmHighLevel, d.meanAlarmSetting.level);
//        value = QVariant(level);
//        moduleManager->setModuleConfig(mLabelId, IBPC_MeanAlarmNormalLevel, value);

//        value = QVariant(d.meanAlarmSetting.limit[CmsAlarmConfig<>::up_index]);
//        moduleManager->setModuleConfig(mLabelId, IBPC_MeanAlarmNormalHighLimit, value);

//        value = QVariant(d.meanAlarmSetting.limit[CmsAlarmConfig<>::down_index]);
//        moduleManager->setModuleConfig(mLabelId, IBPC_MeanAlarmNormalLowLimit, value);
//    }

    return true;
}


