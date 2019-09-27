#include "cmsarrcontenthandler.h"
#include "cmsarrdata.h"
#include "module/valueid.h"
#include <QDebug>
#include <QVariant>

CmsArrContentHandler::CmsArrContentHandler(LabelID label, QObject *parent)
    : CmsContentHandler(label, parent)
{

}

CmsArrContentHandler::~CmsArrContentHandler()
{

}

bool CmsArrContentHandler::handleContent(CmsMessageType type, const QByteArray &arr)
{
    bool ret = false;
    switch (type)
    {
        case CMS_MessageUpArrInfo:
            ret = handleArrData(arr);
            break;
        default:
            break;
    }

    return ret;
}

bool CmsArrContentHandler::handleModuleData(const QByteArray &arr)
{
    Q_UNUSED(arr);
    return true;
}

bool CmsArrContentHandler::handleConfigData(const QByteArray &arr)
{
    Q_UNUSED(arr);
    return true;
}

bool CmsArrContentHandler::handleArrData(const QByteArray &arr)
{
    auto data = new CmsArrData();

    /*解包，并放在dataField里*/
    if (!data->fromByteArray(arr))
    {
        delete data;
        qDebug() << "ecg handle from byte error!";
        return false;
    }
    /*读出dataField*/
    auto d = data->data();
    delete data;

    /*发送给model*/
    QVariant *pv = new QVariant;
    pv->setValue(d);

    emit sendDataField(mLabelId, pv);

    return true;
}
