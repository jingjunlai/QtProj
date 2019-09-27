#include "cmscontenthandler.h"

CmsContentHandler::CmsContentHandler(LabelID label, QObject *parent) : QObject(parent)
{
    mLabelId = label;
}

CmsContentHandler::~CmsContentHandler()
{

}

bool CmsContentHandler::handleContent(CmsMessageType type, const QByteArray &arr)
{
    bool ret = false;
    switch (type)
    {
        case CMS_MessageUpModuleData:
            ret = handleModuleData(arr);
            break;
        case CMS_MessageUpConfig:
            ret = handleConfigData(arr);
            break;
        default:
            break;
    }

    return ret;
}

void CmsContentHandler::setHandleType(CmsContentHandler::HandleType type)
{
    mHandleType = type;
}

CmsContentHandler::HandleType CmsContentHandler::getHandleType() const
{
    return mHandleType;
}


LabelID CmsContentHandler::getLabelId() const
{
    return mLabelId;
}


