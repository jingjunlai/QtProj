#ifndef SERIALSETTING_H
#define SERIALSETTING_H

#include <QSerialPort>

struct SerialSetting
{
public:
    SerialSetting(QString port,
                  qint32 baudRate,
                  QSerialPort::DataBits dataBits,
                  QSerialPort::Parity parity,
                  QSerialPort::StopBits stopBits);

    SerialSetting();

    ~SerialSetting();

    void setPort(QString port);

    void setBaudRate(qint32 baudRate);

    void setDataBits(QSerialPort::DataBits dataBits);

    void setParity(QSerialPort::Parity parity);

    void setStopBits(QSerialPort::StopBits stopBits);

    void setSeparation(bool separation);

public:
    QString port = "";
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    bool separation;
};

#endif // SERIALSETTING_H
