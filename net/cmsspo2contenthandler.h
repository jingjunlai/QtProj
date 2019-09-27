#ifndef CMSSPO2CONTENTHANDLER_H
#define CMSSPO2CONTENTHANDLER_H

#include "cmscontenthandler.h"

class CmsSpo2ContentHandler : public CmsContentHandler
{
    Q_OBJECT

public:
    explicit CmsSpo2ContentHandler(LabelID label, QObject *parent = nullptr);

    ~CmsSpo2ContentHandler();

protected:

    virtual bool handleModuleData(const QByteArray &arr) Q_DECL_OVERRIDE;

    virtual bool handleConfigData(const QByteArray &arr) Q_DECL_OVERRIDE;
};

#endif // CMSSPO2CONTENTHANDLER_H
