#include "cmsmessagecontent.h"
#include <QDebug>

CmsMessageContent::CmsMessageContent()
{

}

CmsMessageContent &CmsMessageContent::operator =(const QByteArray &stream)
{
    mHeader = CmsMessageHeader();
    mContents.clear();

    if (stream.size() < 18)
        return *this;

    ushort len = ((ushort)stream.at(17) << 8) | (uchar)stream.at(16);
    if (len + 16 + 2 + 1 != stream.size())
        return *this;

    mHeader = stream.mid(0, 16);
    mRawData = stream.mid(18, stream.size() - 19);      // 19 = 16(header) + 2(length) + 1(check sum)
//    decodeMessage(stream.mid(18, stream.size() - 19));

    return *this;
}

CmsMessageContent &CmsMessageContent::operator =(const CmsMessageContent &other)
{
    mHeader = other.mHeader;
    mRawData = other.mRawData;
    mContents = other.mContents;
    return *this;
}

CmsMessageHeader CmsMessageContent::header() const
{
    return mHeader;
}

void CmsMessageContent::setHeader(const CmsMessageHeader &header)
{
    mHeader = header;
}

void CmsMessageContent::addContent(QSharedPointer<ICmsData> content)
{
    mContents.append(content);
}

QList<QSharedPointer<ICmsData> > CmsMessageContent::contents() const
{
    return mContents;
}

QByteArray CmsMessageContent::receivedContentArray() const
{
    return mRawData;
}

void CmsMessageContent::clear()
{
    mContents.clear();
}
