#include "cmscommandhandler.h"


#include "module/valueid.h"
#include <QDebug>
#include <QVariant>

CmsCommandHandler::CmsCommandHandler(LabelID label, QObject *parent)
    : CmsContentHandler(label, parent)
{

}

CmsCommandHandler::~CmsCommandHandler()
{

}

bool CmsCommandHandler::handleContent(CmsMessageType type, const QByteArray &arr)
{
    bool ret = false;
    switch (type)
    {
        case CMS_MessageUpCommand:
            ret = handleCommandData(arr);
            break;
        default:
            break;
    }

    return ret;
}

bool CmsCommandHandler::handleModuleData(const QByteArray &arr)
{
    Q_UNUSED(arr);
    return true;
}

bool CmsCommandHandler::handleConfigData(const QByteArray &arr)
{
    Q_UNUSED(arr);
    return true;
}

bool CmsCommandHandler::handleCommandData(const QByteArray &arr)
{
    CmsModuleID identifier = (CmsModuleID)arr.at(0);
    CmsLabelID label = (CmsLabelID)arr.at(1);

    auto data = new CmsSysCommand(identifier, label);

    /*解包，并放在dataField里*/
    if (!data->fromByteArray(arr))
    {
        delete data;
        qDebug() << "command handle from byte error!";
        return false;
    }
    /*读出dataField*/
    auto d = data->command();
    delete data;

    /*发送给model*/
    emit sendUserType((int)d.user.type);

    return true;
}
