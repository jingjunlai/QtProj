#ifndef ISERIALDATAHANDLER_H
#define ISERIALDATAHANDLER_H


class ProtocolPacket;

class ISerialDataHandler
{
public:

    explicit ISerialDataHandler();

    virtual void parseData(const ProtocolPacket &packet) = 0;

    virtual ~ISerialDataHandler()
    {
    }
};

#endif // ISERIALDATAHANDLER_H
