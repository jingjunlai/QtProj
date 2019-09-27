#include "cmsmessageheader.h"

CmsMessageHeader::CmsMessageHeader(Byte localMachineNo,
                                   Byte peerMachineNo,
                                   CmsMessageType msgType)
    : CmsMessageHeader()
{
    mLocalMachineNo = localMachineNo;
    mPeerMachineNo = peerMachineNo;
    mMessageType = msgType;
}

CmsMessageHeader::CmsMessageHeader(Byte localGroupNo,
                                   Byte localMachineNo,
                                   Byte peerGroupNo,
                                   Byte peerMachineNo,
                                   CmsMessageType msgType)
    : CmsMessageHeader()
{
    mLocalGroupNo = localGroupNo;
    mLocalMachineNo = localMachineNo;

    mPeerGroupNo = peerGroupNo;
    mPeerMachineNo = peerMachineNo;

    mMessageType = msgType;
}

CmsMessageHeader::CmsMessageHeader()
{
    mHeader.resize(2);
    mHeader[0] = CMS_PACKET_HEADER1;
    mHeader[1] = CMS_PACKET_HEADER2;
}

CmsMessageHeader &CmsMessageHeader::operator =(const QByteArray &arr)
{
    mHeader = arr.left(2);
    mMessageType = (CmsMessageType)((Byte)arr.at(2));
    mLocalGroupNo = (Byte)arr.at(3);
    mLocalMachineNo = (Byte)arr.at(4);
    mPeerGroupNo = (Byte)arr.at(5);
    mPeerMachineNo = (Byte)arr.at(6);
    Byte version = arr.at(7);
    mMajorVersion = version & 0x0f;
    mMinorVersion = version >> 4;
    mTransmitType = (DataTransmitType)(arr.at(8) & 0x03);

    return *this;
}

void CmsMessageHeader::setLocalGroupNo(Byte groupNo)
{
    mLocalGroupNo = groupNo;
}

Byte CmsMessageHeader::localGroupNo() const
{
    return mLocalGroupNo;
}

void CmsMessageHeader::setLocalMachineNo(Byte machineNo)
{
    mLocalMachineNo = machineNo;
}

Byte CmsMessageHeader::localMachineNo() const
{
    return mLocalMachineNo;
}

void CmsMessageHeader::setPeerGroupNo(Byte groupNo)
{
    mPeerGroupNo = groupNo;
}

Byte CmsMessageHeader::peerGroupNo() const
{
    return mPeerGroupNo;
}

void CmsMessageHeader::setPeerMachineNo(Byte machineNo)
{
    mPeerMachineNo = machineNo;
}

Byte CmsMessageHeader::peerMachineNo() const
{
    return mPeerMachineNo;
}

void CmsMessageHeader::setMessageType(CmsMessageType type)
{
    mMessageType = type;
}

CmsMessageType CmsMessageHeader::messageType() const
{
    return mMessageType;
}

void CmsMessageHeader::setTransmitType(DataTransmitType type)
{
    mTransmitType = type;
}

CmsMessageHeader::DataTransmitType CmsMessageHeader::transmitType() const
{
    return mTransmitType;
}

QByteArray CmsMessageHeader::toByteArray() const
{
    unsigned char buf[16];
    memset(buf, 0, sizeof(buf));
    buf[0] = mHeader.at(0);
    buf[1] = mHeader.at(1);
    buf[2] = (Byte)mMessageType;
    buf[3] = (Byte)mLocalGroupNo;
    buf[4] = (Byte)mLocalMachineNo;
    buf[5] = (Byte)mPeerGroupNo;
    buf[6] = (Byte)mPeerMachineNo;
    buf[7] = (Byte)((mMajorVersion << 4) + mMinorVersion);

    switch (mTransmitType)
    {
        default:
        case OriginData:
            buf[8] = 0;
            break;
        case Encryption:
            buf[8] = 1;
            break;
        case Compress:
            buf[8] = 2;
            break;
        case EncryptionAndCompress:
            buf[8] = 3;
            break;
    }

    return QByteArray((char*)buf, sizeof(buf));
}

void CmsMessageHeader::setMajor(Byte majorVersion)
{
    mMajorVersion = majorVersion;
}

void CmsMessageHeader::setMinor(Byte minorVersion)
{
    mMinorVersion = minorVersion;
}

QString CmsMessageHeader::protocolVersion() const
{
    return QString("%1.%2").arg(mMajorVersion).arg(mMinorVersion);
}

bool CmsMessageHeader::needResponse() const
{
    return mNeedAck;
}

