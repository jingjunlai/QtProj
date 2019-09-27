#ifndef CMSMESSAGEHEADER_H
#define CMSMESSAGEHEADER_H

#include "cmsprotocolparser.h"
#include "cmstype.h"

#include <QByteArray>
#include <QFlags>

class CmsMessageHeader
{
public:
    enum DataTransmitType
    {
        OriginData = 0,
        Encryption = 1,
        Compress = 2,
        EncryptionAndCompress = 3
    };

public:
    CmsMessageHeader(Byte localMachineNo,
                     Byte peerMachineNo,
                     CmsMessageType msgType);

    CmsMessageHeader(Byte localGroupNo,
                     Byte localMachineNo,
                     Byte peerGroupNo,
                     Byte peerMachineNo,
                     CmsMessageType msgType);

    CmsMessageHeader();

    CmsMessageHeader &operator =(const QByteArray &arr);

    void setLocalGroupNo(Byte groupNo);

    Byte localGroupNo() const;

    void setLocalMachineNo(Byte machineNo);

    Byte localMachineNo() const;

    void setPeerGroupNo(Byte groupNo);

    Byte peerGroupNo() const;

    void setPeerMachineNo(Byte machineNo);

    Byte peerMachineNo() const;

    void setMessageType(CmsMessageType type);

    CmsMessageType  messageType() const;

    void setTransmitType(DataTransmitType type);

    DataTransmitType  transmitType() const;

    QByteArray toByteArray() const;

    void setMajor(Byte majorVersion);

    void setMinor(Byte minorVersion);

    QString protocolVersion() const;

    bool needResponse() const;

private:
    QByteArray          mHeader{2, 0};
    CmsMessageType      mMessageType;
    Byte                mLocalGroupNo = 0;
    Byte                mLocalMachineNo = 0;
    Byte                mPeerGroupNo = 0;
    Byte                mPeerMachineNo = 0;
    Byte                mMajorVersion = 1;
    Byte                mMinorVersion = 0;
    DataTransmitType    mTransmitType = OriginData;

    bool                mNeedAck = false;
};

#endif // CMSMESSAGEHEADER_H
