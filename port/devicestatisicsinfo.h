#ifndef DEVICESTATISICSINFO_H
#define DEVICESTATISICSINFO_H

struct DeviceStatisticsInfo
{
    DeviceStatisticsInfo()
    {
        init();
    }

    void init()
    {
        receivedTotalBytes = 0;
        receivedCorrectBytes = 0;
        receivedErrorPackets = 0;
        receivedCorrectPackets = 0;

        receivedBytesPerSecond = 0;
        receivedCorrectBytesPerSecond = 0;
        receivedErrorPacketsPerSecond = 0;
        receivedCorrectPacketsPerSecond = 0;

        sendTotalBytes = 0;
        sendPackets = 0;

        sendBytesPerSecond = 0;
        sendPacketsPerSecond = 0;
    }

    long long receivedTotalBytes;
    long long receivedCorrectBytes;
    long long receivedErrorPackets;
    long long receivedCorrectPackets;

    long long receivedBytesPerSecond;
    long long receivedCorrectBytesPerSecond;
    long long receivedErrorPacketsPerSecond;
    long long receivedCorrectPacketsPerSecond;

    long long sendTotalBytes;
    long long sendPackets;

    long long sendBytesPerSecond;
    long long sendPacketsPerSecond;
};

#endif // DEVICESTATISICSINFO_H
