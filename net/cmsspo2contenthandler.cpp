#include "cmsspo2contenthandler.h"
#include "cmsspo2moduledata.h"
#include "cmsspo2configdata.h"
#include "module/moduledata.h"
#include "module/valueid.h"
#include <QDebug>
#include <QVariant>

CmsSpo2ContentHandler::CmsSpo2ContentHandler(LabelID label, QObject *parent)
    : CmsContentHandler(label, parent)
{

}

CmsSpo2ContentHandler::~CmsSpo2ContentHandler()
{

}

bool CmsSpo2ContentHandler::handleModuleData(const QByteArray &arr)
{
//    qDebug() << "spo2 moudule data = " << arr.toHex();

    CmsModuleID identifier = (CmsModuleID)arr.at(0);
    CmsLabelID label = (CmsLabelID)arr.at(1);
    auto data = new CmsSpo2ModuleData(identifier, label);
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
    auto d = ((CmsSpo2ModuleData*)data)->data();
    delete data;

    /*发送给model*/
    QVariant *pv = new QVariant;
    pv->setValue(d);

    emit sendDataField(mLabelId, pv);

//    ParamPoint param;
//    param.label = LABEL_SPO2;
//    param.subid = Sub_Spo2;
//    param.data = d.spo2;
//    mModuleData.params.append(param);

//    QList<WavePoint> wave;
//    for (int i = 0; i < d.waves.size(); i++)
//    {
//        auto frameData = d.waves.at(i);
//        WavePoint tmp;
//        tmp.data = frameData.spo2Wave;
//        wave.append(tmp);
//    }

//    mModuleData.waves.insert(WAVE_SPO2, wave);

    return true;
}

bool CmsSpo2ContentHandler::handleConfigData(const QByteArray &arr)
{
    if (arr.size() < 2)
        return false;

    //qDebug() << "spo2 config data = " << arr.toHex();

    CmsModuleID identifier = (CmsModuleID)arr.at(0);
    CmsLabelID label = (CmsLabelID)arr.at(1);
    auto conf = new CmsSpo2ConfigData(identifier, label);
    if (conf == Q_NULLPTR)
        return false;

    if (!conf->fromByteArray(arr))
        return false;

    auto d = conf->config();
    delete conf;

    // save
    QVariant value;
    value = QVariant(d.spo2AlarmSetting.sw ? Gps::On : Gps::Off);
    //moduleManager->setModuleConfig(mLabelId, SC_SpO2AlarmNormalSwitch, value);

    int level = qMin(CMS_AlarmHighLevel, d.spo2AlarmSetting.level);
    value = QVariant(level);
    //moduleManager->setModuleConfig(mLabelId, SC_SpO2AlarmNormalLevel, value);

    value = QVariant(d.spo2AlarmSetting.highLimt);
    //moduleManager->setModuleConfig(mLabelId, SC_SpO2AlarmNormalHighLimit, value);

    value = QVariant(d.spo2AlarmSetting.lowLimit);
    //moduleManager->setModuleConfig(mLabelId, SC_SpO2AlarmNormalLowLimit, value);

    level = qMin(CMS_AlarmHighLevel, d.spo2AlarmSetting.destLevel);
    value = QVariant(level);
    //moduleManager->setModuleConfig(mLabelId, SC_SpO2DesatAlarmLevel, value);

    return true;
}
