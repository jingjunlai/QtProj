#ifndef CMSRESPCONTENTHANDLER_H
#define CMSRESPCONTENTHANDLER_H

#include "cmscontenthandler.h"

class CmsRespContentHandler : public CmsContentHandler
{
    Q_OBJECT
public:

    explicit CmsRespContentHandler(LabelID label, QObject *parent = nullptr);

    ~CmsRespContentHandler();

protected:

    virtual bool handleModuleData(const QByteArray &arr) Q_DECL_OVERRIDE;

    virtual bool handleConfigData(const QByteArray &arr) Q_DECL_OVERRIDE;


private:

};

#endif // CMSRESPCONTENTHANDLER_H
