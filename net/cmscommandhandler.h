#ifndef CMSCOMMANDHANDLER_H
#define CMSCOMMANDHANDLER_H


#include "cmscontenthandler.h"
#include "cmssyscommand.h"

class CmsCommandHandler : public CmsContentHandler
{
    Q_OBJECT
public:

    explicit CmsCommandHandler(LabelID label, QObject *parent = nullptr);

    ~CmsCommandHandler();

    virtual bool handleContent(CmsMessageType type, const QByteArray &arr);

signals:

    void sendUserType(int type);

protected:

    virtual bool handleModuleData(const QByteArray &arr);

    virtual bool handleConfigData(const QByteArray &arr);

    virtual bool handleCommandData(const QByteArray &arr);

private:

};

#endif // CMSCOMMANDHANDLER_H
