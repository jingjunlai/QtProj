#ifndef CMSMESSAGECONTENT_H
#define CMSMESSAGECONTENT_H

#include "cmsmessageheader.h"
#include "icmsdata.h"

#include <QSharedPointer>
#include <QList>

class CmsMessageContent
{
public:
    CmsMessageContent();

    CmsMessageContent &operator =(const QByteArray &stream);

    CmsMessageContent &operator =(const CmsMessageContent &other);

    CmsMessageHeader header() const;

    void setHeader(const CmsMessageHeader &header);

    void addContent(QSharedPointer<ICmsData> content);

    QList<QSharedPointer<ICmsData>> contents() const;

    QByteArray receivedContentArray() const;

    void clear();

private:

    CmsMessageHeader                    mHeader;   //报文头
    QByteArray                          mRawData;  //原始数据
    QList<QSharedPointer<ICmsData>>     mContents;  //组包好的数据

};

#endif // CMSMESSAGECONTENT_H
