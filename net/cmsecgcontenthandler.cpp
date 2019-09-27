#include "cmsecgcontenthandler.h"
#include "cmsecgmoduledata.h"
#include "cmsecgconfigdata.h"
#include "module/moduledata.h"
#include "module/valueid.h"
#include <QDebug>
#include <QVariant>

CmsEcgContentHandler::CmsEcgContentHandler(LabelID label, QObject *parent)
    : CmsContentHandler(label, parent)
{

}

CmsEcgContentHandler::~CmsEcgContentHandler()
{

}

bool CmsEcgContentHandler::handleModuleData(const QByteArray &arr)
{
    //qDebug() << "ecg moudule data = " << arr.toHex();

    CmsModuleID identifier = (CmsModuleID)arr.at(0);
    CmsLabelID label = (CmsLabelID)arr.at(1);

    auto data = new CmsEcgModuleData(identifier, label);
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
    auto d = ((CmsEcgModuleData*)data)->data();
    delete data;

    /*发送给model*/
    QVariant *pv = new QVariant;
    pv->setValue(d);

    emit sendDataField(mLabelId, pv);


    return true;
}

bool CmsEcgContentHandler::handleConfigData(const QByteArray &arr)
{

    //qDebug() << "ecg config data = " << arr.toHex();

    if (arr.size() < 2)
        return false;

    CmsModuleID identifier = (CmsModuleID)arr.at(0);
    CmsLabelID label = (CmsLabelID)arr.at(1);
    auto conf = new CmsEcgConfigData(identifier, label);
    if (conf == Q_NULLPTR)
        return false;

    if (!conf->fromByteArray(arr))
        return false;

    auto d = conf->config();
    delete conf;

    /*发送给model*/
    QVariant *pv = new QVariant;
    pv->setValue(d);

    emit sendConfigField(mLabelId, pv);


    // save
    QVariant value;
    value = QVariant(d.hrAlarmSetting.sw ? Gps::On : Gps::Off);
    //moduleManager->setModuleConfig(mLabelId, ECGC_HRAlarmNormalSwitch, value);

    int level = qMin(CMS_AlarmHighLevel, d.hrAlarmSetting.level);
    value = QVariant(level);
    //moduleManager->setModuleConfig(mLabelId, ECGC_HRAlarmNormalLevel, value);

    value = QVariant(d.hrAlarmSetting.limit[CmsAlarmConfig<>::up_index]);
    //moduleManager->setModuleConfig(mLabelId, ECGC_HRAlarmNormalHighLimit, value);

    value = QVariant(d.hrAlarmSetting.limit[CmsAlarmConfig<>::down_index]);
    //moduleManager->setModuleConfig(mLabelId, ECGC_HRAlarmNormalLowLimit, value);
    return true;
}



