#ifndef CMSECGCONTENTHANDLER_H
#define CMSECGCONTENTHANDLER_H

#include "cmscontenthandler.h"


class CmsEcgContentHandler : public CmsContentHandler
{
    Q_OBJECT
public:

    explicit CmsEcgContentHandler(LabelID label, QObject *parent = nullptr);

    ~CmsEcgContentHandler();

protected:

    virtual bool handleModuleData(const QByteArray &arr) Q_DECL_OVERRIDE;

    virtual bool handleConfigData(const QByteArray &arr) Q_DECL_OVERRIDE;


private:

};

#endif // CMSECGCONTENTHANDLER_H
