#ifndef NBMPROTOCOLPARSER_H
#define NBMPROTOCOLPARSER_H

#include "iprotocolparser.h"

#define NBM_PACKET_HEADER1          0xaa
#define NBM_PACKET_HEADER2          0xab
#define NBM_PAYLOAD_START_BYTES     3

class NbmProtocolParser : public IProtocolParser
{
public:
    explicit NbmProtocolParser();

    virtual ~NbmProtocolParser();

    virtual QByteArray makePacket(const unsigned char* pData, int len);

protected:

    virtual int rebuildPacket(unsigned char* pData, int len);

    virtual bool checkSum(const unsigned char* pData, int len);
};

#endif // NBMPROTOCOLPARSER_H
