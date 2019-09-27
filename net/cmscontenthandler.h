#ifndef CMSCONTENTHANDLER_H
#define CMSCONTENTHANDLER_H

#include "icmsdata.h"
#include "cmstype.h"
#include "module/moduleid.h"

#include <QObject>

class CmsContentHandler : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief The HandleType enum 数据处理类型,是全部处理,还是部分处理.
     * 绝大多数的模块数据不区分该类型.
     */
    enum HandleType
    {
        HandleMaximum,      // 处理所有数据

        /*
         * 针对心电模块,每次会上传很多波形数据,如果:
         *  作为中央机,应该全部处理.
         *  作为床边机,只处理主要导联数据即可,提高软件运行效率.
         */
        HandleMinimum,      // 处理最低限度的数据,仅仅是必要的
    };

    explicit CmsContentHandler(LabelID label, QObject *parent = 0);

    ~CmsContentHandler();

    virtual bool handleContent(CmsMessageType type, const QByteArray &arr);

    void setHandleType(HandleType type);

    HandleType getHandleType() const;

    LabelID getLabelId() const;

signals:

    void sendDataField(LabelID label, const QVariant *value);

    void sendConfigField(LabelID label, const QVariant *value);

protected:

    virtual bool handleModuleData(const QByteArray &arr) = 0;

    virtual bool handleConfigData(const QByteArray &arr) = 0;

    LabelID                 mLabelId = LABEL_NULL;
    HandleType              mHandleType = HandleMaximum;

};

#endif // CMSCONTENTHANDLER_H
