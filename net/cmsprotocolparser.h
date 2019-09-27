#ifndef CMSPROTOCOLPARSER_H
#define CMSPROTOCOLPARSER_H

#include "port/iprotocolparser.h"

#define CMS_PACKET_HEADER1          0xAA
#define CMS_PACKET_HEADER2          0xAB

class CmsProtocolParser : public IProtocolParser
{


public:
    explicit CmsProtocolParser();

    ~CmsProtocolParser();

    virtual QByteArray makePacket(const unsigned char *pucData, int iLen);

protected:

    virtual int rebuildPacket(unsigned char *pucData, int iLen);

    virtual bool checkSum(const unsigned char *pucData, int iLen);


};

#endif // CMSPROTOCOLPARSER_H
