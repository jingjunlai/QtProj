#ifndef CMSTEMPCONTENTHANDLER_H
#define CMSTEMPCONTENTHANDLER_H

#include "cmscontenthandler.h"

class CmsTempContentHandler : public CmsContentHandler
{
    Q_OBJECT
public:
    explicit CmsTempContentHandler(LabelID label, QObject *parent = nullptr);

    ~CmsTempContentHandler();

protected:

    virtual bool handleModuleData(const QByteArray &arr);

    virtual bool handleConfigData(const QByteArray &arr);
};

#endif // CMSTEMPCONTENTHANDLER_H
