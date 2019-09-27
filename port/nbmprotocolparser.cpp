#include "nbmprotocolparser.h"
#include <QDebug>

NbmProtocolParser::NbmProtocolParser()
    : IProtocolParser(PACKET_MAX_LENGTH, Protocol::Protocol_Nbm)
{

}

NbmProtocolParser::~NbmProtocolParser()
{

}

QByteArray NbmProtocolParser::makePacket(const unsigned char *pData, int len)
{
    // byte0  byte1  byte2  byte3  byte4     ......           byten (n = len + 1)
    //  0xaa   0xab  passID  len    id       content             check sum
    int size = len + 4;
    unsigned char buf[size];
    buf[0] = NBM_PACKET_HEADER1;
    buf[1] = NBM_PACKET_HEADER2;
    buf[2] = pData[0];

    buf[3] = len;
    memcpy(buf + 4, pData + 1, len);

    unsigned char sum = 0;
    for (int i = 3; i < len + 4; i++)
    {
        sum += buf[i];
    }
    buf[size - 1] = sum;

    return QByteArray((char *)buf, size);
}

int NbmProtocolParser::rebuildPacket(unsigned char *pData, int len)
{
    //PrintData(pData, len, "read");
    const unsigned char *pHead = pData;
    const unsigned char *pLastHead = pData;
    int restLen = len;
    int currentPacketLen = 0;
    const unsigned char Packhead[2] = {NBM_PACKET_HEADER1, NBM_PACKET_HEADER2};

    while (restLen > 0)
    {
        pHead = (const unsigned char *)memchr(pLastHead, Packhead[0], restLen);
        if (pHead)
        {
            int dropLen = pHead - pLastHead;
            restLen -= dropLen;
            pLastHead = pHead;

            if (restLen > 1 && Packhead[1] != pHead[1])
            {
                pHead += 1;
                restLen -= 1;
                pLastHead = pHead;
                continue;
            }

            if (restLen > 3)
            {
                currentPacketLen = pHead[3];
            }
            else
            {
                memmove(pData, pHead, restLen);
                return restLen;
            }

            if (currentPacketLen + 4 > restLen)
            {
                memmove(pData, pHead, restLen);
                return restLen;
            }
            else
            {
                if (checkSum(pHead, currentPacketLen + 4))
                {
                    QByteArray packet(QByteArray((char *)pHead, currentPacketLen + 4));
                    m_LegalPacketList.push_back(packet);
                    pHead += currentPacketLen + 4;
                    restLen -= currentPacketLen + 4;
                    pLastHead = pHead;
                    m_llCorrectPackets++;
                }
                else
                {
//                    qDebug() << QString("check sum error, read bytes:%1").arg(len);
//                    PrintData(pData, len, "read");
//                    PrintData(pHead, currentPacketLen + 4, "check sum error");
                    pHead += 1;
                    restLen -= 1;
                    pLastHead = pHead;
                    m_llErrorPackets++;
                    qWarning() << QString("Nbm protocol error packets:%1.").arg(m_llErrorPackets);
                }
            }

        }
        else
        {
            return 0;
        }
    }
    return restLen;
}

bool NbmProtocolParser::checkSum(const unsigned char *pData, int len)
{
    if (pData && len > 6)
    {
        unsigned char packetSum = pData[len - 1];
        unsigned char checkSum = 0;
        for (int i = 3; i < len - 1; i++)
        {
            checkSum += pData[i];
        }

        if (packetSum == checkSum)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
