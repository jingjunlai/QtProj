#include "iprotocolparser.h"

IProtocolParser::IProtocolParser(int iBuffersize, Protocol::ProtocolType type)
    : m_iMallocBufferCounts(iBuffersize)
    , m_ProtocolType(type)
{
    m_pBuffer = new unsigned char[m_iMallocBufferCounts];
    m_iBufferCounts = 0;
    m_llCorrectPackets = 0;
    m_llErrorPackets = 0;
}

IProtocolParser::~IProtocolParser()
{
    if (m_pBuffer)
        delete [] m_pBuffer;
}

Protocol::ProtocolType IProtocolParser::getProtocolType() const
{
    return m_ProtocolType;
}

void IProtocolParser::appendData(unsigned char *pData, int len)
{
    int restLen = len;
    const unsigned char* p = pData;
    while (true)
    {
        int uselen = 0;
        if (m_iBufferCounts + restLen > m_iMallocBufferCounts)
        {
            uselen = m_iMallocBufferCounts - m_iBufferCounts;
        }
        else
        {
            uselen = restLen;
        }
        restLen -= uselen;

        memcpy(m_pBuffer + m_iBufferCounts, p, uselen);
        p += uselen;
        if (uselen > 0)
        {
            m_iBufferCounts += uselen;
            m_iBufferCounts = rebuildPacket(m_pBuffer, m_iBufferCounts);
        }

        if (restLen == 0)
            break;
    }
}

QByteArray IProtocolParser::makePacket(const QByteArray &data)
{
    return makePacket((unsigned char *)data.constData(), data.size());
}

int IProtocolParser::freeBytes()
{
    return m_iMallocBufferCounts - m_iBufferCounts;
}

QVector<QByteArray> IProtocolParser::getLegalPackets()
{
    QVector<QByteArray> ret = m_LegalPacketList;
    m_LegalPacketList.clear();
    return ret;
}

long long IProtocolParser::getCorrectPacketCounts() const
{
    return m_llCorrectPackets;
}

long long IProtocolParser::getErrorPacketCounts() const
{
    return m_llErrorPackets;
}
