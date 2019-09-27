#ifndef CMSIBPCONTENTHANDLER_H
#define CMSIBPCONTENTHANDLER_H

#include "cmscontenthandler.h"

class CmsIbpContentHandler : public CmsContentHandler
{
    Q_OBJECT
public:
    explicit CmsIbpContentHandler(LabelID label, QObject *parent = nullptr);

    ~CmsIbpContentHandler();

protected:

    virtual bool handleModuleData(const QByteArray &arr);

    virtual bool handleConfigData(const QByteArray &arr);
};

#endif // CMSIBPCONTENTHANDLER_H
