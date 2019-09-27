#ifndef IPROTOCOLPARSER_H
#define IPROTOCOLPARSER_H

#include <QVector>
#include "module/moduleproperty.h"

#define PACKET_MAX_LENGTH           10000

class IProtocolParser
{

public:

    explicit IProtocolParser(int iBuffersize, Protocol::ProtocolType type);

    virtual ~IProtocolParser();

    Protocol::ProtocolType getProtocolType() const;

    void appendData(unsigned char *, int);

    virtual QByteArray makePacket(const unsigned char *, int) = 0;

    virtual QByteArray makePacket(const QByteArray &data);

    int freeBytes();

    QVector<QByteArray> getLegalPackets();

    long long getCorrectPacketCounts() const;

    long long getErrorPacketCounts() const;

protected:

    virtual int rebuildPacket(unsigned char *, int) = 0;

    virtual bool checkSum(const unsigned char *, int) = 0;

    QVector<QByteArray>         m_LegalPacketList;
    unsigned char               *m_pBuffer = Q_NULLPTR;
    int                         m_iMallocBufferCounts = 0;
    int                         m_iBufferCounts = 0;
    long long                   m_llCorrectPackets = 0;
    long long                   m_llErrorPackets = 0;
    Protocol::ProtocolType      m_ProtocolType = Protocol::Protocol_Unknown;
};

struct ProtocolPacket
{
    Protocol::ProtocolType      protocol;
    QByteArray                  data;
};

#endif // IPROTOCOLPARSER_H
