#include "cmsrespcontenthandler.h"
#include "cmsrespmoduledata.h"
#include "cmsrespconfigdata.h"
#include "module/moduledata.h"
#include "module/valueid.h"
#include <QDebug>
#include <QVariant>

CmsRespContentHandler::CmsRespContentHandler(LabelID label, QObject *parent)
    : CmsContentHandler(label, parent)
{

}

CmsRespContentHandler::~CmsRespContentHandler()
{

}

bool CmsRespContentHandler::handleModuleData(const QByteArray &arr)
{
    //qDebug() << "ecg moudule data = " << arr.toHex();

    CmsModuleID identifier = (CmsModuleID)arr.at(0);
    CmsLabelID label = (CmsLabelID)arr.at(1);

    auto data = new CmsRespModuleData(identifier, label);
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
    auto d = ((CmsRespModuleData*)data)->data();
    delete data;

    /*发送给model*/
    QVariant *pv = new QVariant;
    pv->setValue(d);

    emit sendDataField(mLabelId, pv);

//    ParamPoint param;
//    param.label = LABEL_RESP;
//    param.subid = Sub_Resp;
//    param.data = d.respRate;
//    mModuleData.params.append(param);

//    QList<WavePoint> wave;
//    for (int i = 0; i < d.waves.size(); i++)
//    {
//        auto frameData = d.waves.at(i);
//        WavePoint tmp;
//        tmp.data = frameData.respWave;
//        wave.append(tmp);
//    }

//    mModuleData.waves.insert(WAVE_RESP, wave);

    return true;
}

bool CmsRespContentHandler::handleConfigData(const QByteArray &arr)
{
    if (arr.size() < 2)
        return false;

    CmsModuleID identifier = (CmsModuleID)arr.at(0);
    CmsLabelID label = (CmsLabelID)arr.at(1);
    auto conf = new CmsRespConfigData(identifier, label);
    if (conf == Q_NULLPTR)
        return false;

    if (!conf->fromByteArray(arr))
        return false;

    auto d = conf->config();
    delete conf;

    // save
    QVariant value;
    value = QVariant(d.rrAlarmSetting.sw ? Gps::On : Gps::Off);
    //moduleManager->setModuleConfig(mLabelId, RC_RespAlarmNormalSwitch, value);

    int level = qMin(CMS_AlarmHighLevel, d.rrAlarmSetting.level);
    value = QVariant(level);
    //moduleManager->setModuleConfig(mLabelId, RC_RespAlarmNormalLevel, value);

    value = QVariant(d.rrAlarmSetting.limit[CmsAlarmConfig<>::up_index]);
    //moduleManager->setModuleConfig(mLabelId, RC_RespAlarmNormalHighLimit, value);

    value = QVariant(d.rrAlarmSetting.limit[CmsAlarmConfig<>::down_index]);
    //moduleManager->setModuleConfig(mLabelId, RC_RespAlarmNormalLowLimit, value);

    return true;
}

