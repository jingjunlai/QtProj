#ifndef ISERIALPORT_H
#define ISERIALPORT_H

#include <QObject>
#include "serialsetting.h"
#include "devicestatisicsinfo.h"

class IProtocolParser;
class ISerialDataHandler;
class ISerialPort : public QObject
{
    Q_OBJECT
public:

    explicit ISerialPort(const SerialSetting &setting, QObject *parent = nullptr);

    virtual ~ISerialPort();

    DeviceStatisticsInfo getStatisticsInfo() const;

    void setParser(IProtocolParser *pParser);

    void setSerialSetting(const SerialSetting &setting);

    void setDataHandler(ISerialDataHandler *handler);

public:

    virtual bool open(QIODevice::OpenMode mode) = 0;

    virtual void close() = 0;

    virtual qint64 write(const QByteArray &data) = 0;

    virtual bool isOpen() const = 0;

public slots:

    void start();

protected:
    SerialSetting           mSetting;
    IProtocolParser         *mParser = Q_NULLPTR;
    ISerialDataHandler      *mDataHandler = Q_NULLPTR;
    DeviceStatisticsInfo    mStatisticsInfo[2];
};

#endif // ISERIALPORT_H
