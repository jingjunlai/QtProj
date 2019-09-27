#ifndef CMSNIBPCONTENTHANDLER_H
#define CMSNIBPCONTENTHANDLER_H

#include "cmscontenthandler.h"

class CmsNibpContentHandler : public CmsContentHandler
{
    Q_OBJECT
public:
    explicit CmsNibpContentHandler(LabelID label, QObject *parent = nullptr);

    ~CmsNibpContentHandler();

protected:

    virtual bool handleModuleData(const QByteArray &arr);

    virtual bool handleConfigData(const QByteArray &arr);
};

#endif // CMSNIBPCONTENTHANDLER_H
