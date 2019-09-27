#ifndef CMSCO2CONTENTHANDLER_H
#define CMSCO2CONTENTHANDLER_H

#include "cmscontenthandler.h"

class CmsCo2ContentHandler : public CmsContentHandler
{
    Q_OBJECT
public:
    explicit CmsCo2ContentHandler(LabelID label, QObject *parent = nullptr);

    ~CmsCo2ContentHandler();

protected:

    virtual bool handleModuleData(const QByteArray &arr);

    virtual bool handleConfigData(const QByteArray &arr);
};

#endif // CMSCO2CONTENTHANDLER_H
