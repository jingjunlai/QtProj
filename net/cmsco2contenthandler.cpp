#include "cmsco2contenthandler.h"
#include "cmsco2moduledata.h"
#include "cmsco2configdata.h"
#include "module/moduledata.h"
#include "module/valueid.h"
#include <QDebug>
#include <QVariant>

CmsCo2ContentHandler::CmsCo2ContentHandler(LabelID label, QObject *parent)
    : CmsContentHandler(label, parent)
{

}

CmsCo2ContentHandler::~CmsCo2ContentHandler()
{

}

bool CmsCo2ContentHandler::handleModuleData(const QByteArray &arr)
{
    //qDebug() << "co2 moudule data = " << arr.toHex();

    CmsModuleID identifier = (CmsModuleID)arr.at(0);
    CmsLabelID label = (CmsLabelID)arr.at(1);

    auto data = new CmsCo2ModuleData(identifier, label);
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
    auto d = ((CmsCo2ModuleData*)data)->data();
    delete data;

    /*发送给model*/
    QVariant *pv = new QVariant;
    pv->setValue(d);

    emit sendDataField(mLabelId, pv);

//    ParamPoint param;
//    param.label = LABEL_CO2;
//    param.subid = Sub_CO2Et;
//    param.data = d.etValue;
//    mModuleData.params.append(param);

//    param.label = LABEL_CO2;
//    param.subid = Sub_CO2Fi;
//    param.data = d.fiValue;
//    mModuleData.params.append(param);

//    param.label = LABEL_CO2;
//    param.subid = Sub_CO2awRR;
//    param.data = d.awrrValue;
//    mModuleData.params.append(param);

//    QList<WavePoint> wave;
//    for (int i = 0; i < d.waves.size(); i++)
//    {
//        auto frameData = d.waves.at(i);
//        WavePoint tmp;
//        tmp.data = frameData.co2Wave;
//        wave.append(tmp);
//    }

//    mModuleData.waves.insert(WAVE_CO2, wave);

    return true;
}

bool CmsCo2ContentHandler::handleConfigData(const QByteArray &arr)
{
    //qDebug() << "ecg config data = " << arr.toHex();

    if (arr.size() < 2)
        return false;

    CmsModuleID identifier = (CmsModuleID)arr.at(0);
    CmsLabelID label = (CmsLabelID)arr.at(1);
    auto conf = new CmsCo2ConfigData(identifier, label);
    if (conf == Q_NULLPTR)
        return false;

    if (!conf->fromByteArray(arr))
        return false;

    auto d = conf->config();
    delete conf;

    // save
    // et alarm
    {
        QVariant value;
        value = QVariant(d.etCo2AlarmSetting.sw ? Gps::On : Gps::Off);
//        moduleManager->setModuleConfig(mLabelId, CC_CO2EtAlarmNormalSwitch, value);

        int level = qMin(CMS_AlarmHighLevel, d.etCo2AlarmSetting.level);
        value = QVariant(level);
//        moduleManager->setModuleConfig(mLabelId, CC_CO2EtAlarmNormalLevel, value);

        value = QVariant(d.etCo2AlarmSetting.limit[CmsAlarmConfig<>::up_index]);
//        moduleManager->setModuleConfig(mLabelId, CC_CO2EtAlarmNormalHighLimit, value);

        value = QVariant(d.etCo2AlarmSetting.limit[CmsAlarmConfig<>::down_index]);
//        moduleManager->setModuleConfig(mLabelId, CC_CO2EtAlarmNormalLowLimit, value);
    }

    // fi alarm
    {
        QVariant value;
        value = QVariant(d.fiCo2AlarmSetting.sw ? Gps::On : Gps::Off);
//        moduleManager->setModuleConfig(mLabelId, CC_CO2FiAlarmNormalSwitch, value);

        int level = qMin(CMS_AlarmHighLevel, d.etCo2AlarmSetting.level);
        value = QVariant(level);
//        moduleManager->setModuleConfig(mLabelId, CC_CO2FiAlarmNormalLevel, value);

        value = QVariant(d.fiCo2AlarmSetting.limit[CmsAlarmConfig<>::up_index]);
//        moduleManager->setModuleConfig(mLabelId, CC_CO2FiAlarmNormalHighLimit, value);

        value = QVariant(d.fiCo2AlarmSetting.limit[CmsAlarmConfig<>::down_index]);
//        moduleManager->setModuleConfig(mLabelId, CC_CO2FiAlarmNormalLowLimit, value);
    }

    // awrr alarm
    {
        QVariant value;
        value = QVariant(d.awrrAlarmSetting.sw ? Gps::On : Gps::Off);
//        moduleManager->setModuleConfig(mLabelId, CC_CO2awRRAlarmNormalSwitch, value);

        int level = qMin(CMS_AlarmHighLevel, d.awrrAlarmSetting.level);
        value = QVariant(level);
//        moduleManager->setModuleConfig(mLabelId, CC_CO2awRRAlarmNormalLevel, value);

        value = QVariant(d.awrrAlarmSetting.limit[CmsAlarmConfig<>::up_index]);
//        moduleManager->setModuleConfig(mLabelId, CC_CO2awRRAlarmNormalHighLimit, value);

        value = QVariant(d.awrrAlarmSetting.limit[CmsAlarmConfig<>::down_index]);
//        moduleManager->setModuleConfig(mLabelId, CC_CO2awRRAlarmNormalLowLimit, value);
    }

    QVariant value;
    value = QVariant(d.apneaTime);
//    moduleManager->setModuleConfig(mLabelId, CC_CO2ApneaDelayTime, value);

    return true;
}


