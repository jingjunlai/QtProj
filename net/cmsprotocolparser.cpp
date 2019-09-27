#include <QDebug>
#include "cmsprotocolparser.h"

CmsProtocolParser::CmsProtocolParser()
    : IProtocolParser(100 * 1000, Protocol::Protocol_Cms)
{

}

CmsProtocolParser::~CmsProtocolParser()
{

}

QByteArray CmsProtocolParser::makePacket(const unsigned char *pucData, int iLen)
{
    int iSize = iLen + 1;
    unsigned char *p = new unsigned char[iSize];
    unsigned char ucSum = 0;


    memcpy(p, pucData, iLen);
    for(int i = 0; i < iLen; i++)
    {
        ucSum += p[i];
    }
    p[iSize - 1] = ucSum;

    auto arr = QByteArray((char *)p, iSize);
    delete [] p;
    return arr;
}

int CmsProtocolParser::rebuildPacket(unsigned char *pucData, int iLen)
{
    const unsigned char *pHead = pucData;
    const unsigned char *pLastHead = pucData;
    int restLen = iLen;
    int currentPacketLen = 0;
    const unsigned char Packhead[2] = {CMS_PACKET_HEADER1, CMS_PACKET_HEADER2};

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

            if (restLen >= 18)
            {
                currentPacketLen = pHead[16] + (pHead[17] << 8);
            }
            else
            {
                memmove(pucData, pHead, restLen);
                return restLen;
            }

            if (currentPacketLen + 19 > restLen)
            {
                memmove(pucData, pHead, restLen);
                return restLen;
            }
            else
            {
                if (checkSum(pHead, currentPacketLen + 19))
                {
                    QByteArray packet(QByteArray((char *)pHead, currentPacketLen + 19));
                    m_LegalPacketList.push_back(packet);
                    pHead += currentPacketLen + 19;
                    restLen -= currentPacketLen + 19;
                    pLastHead = pHead;
                    m_llCorrectPackets++;
                }
                else
                {
                    qDebug() << QString("check sum error, read bytes:%1").arg(iLen);
                    //PrintData(pucData, len, "read");
                    //PrintData(pHead, currentPacketLen + 19, "check sum error");
                    pHead += 1;
                    restLen -= 1;
                    pLastHead = pHead;
                    m_llErrorPackets++;
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

bool CmsProtocolParser::checkSum(const unsigned char *pucData, int iLen)
{
    if (pucData && iLen >= 19)
    {
        unsigned char packetSum = pucData[iLen - 1];
        unsigned char checkSum = 0;
        for (int i = 16; i < iLen - 1; i++)
        {
            checkSum += pucData[i];
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
