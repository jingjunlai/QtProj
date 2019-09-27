#ifndef CMSCOCONTENTHANDLER_H
#define CMSCOCONTENTHANDLER_H

#include "cmscontenthandler.h"

class CmsCoContentHandler : public CmsContentHandler
{
    Q_OBJECT
public:

    explicit CmsCoContentHandler(LabelID label, QObject *parent = nullptr);

    ~CmsCoContentHandler();

protected:

    virtual bool handleModuleData(const QByteArray &arr) Q_DECL_OVERRIDE;

    virtual bool handleConfigData(const QByteArray &arr) Q_DECL_OVERRIDE;


private:

};

#endif // CMSCOCONTENTHANDLER_H
