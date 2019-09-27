#ifndef CMSARRCONTENTHANDLER_H
#define CMSARRCONTENTHANDLER_H


#include "cmscontenthandler.h"


class CmsArrContentHandler : public CmsContentHandler
{
    Q_OBJECT
public:

    explicit CmsArrContentHandler(LabelID label, QObject *parent = nullptr);

    ~CmsArrContentHandler();

    virtual bool handleContent(CmsMessageType type, const QByteArray &arr);

protected:

    virtual bool handleModuleData(const QByteArray &arr);

    virtual bool handleConfigData(const QByteArray &arr);

    virtual bool handleArrData(const QByteArray &arr);

private:

};

#endif // CMSARRCONTENTHANDLER_H
