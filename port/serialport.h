#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include "iserialport.h"

class QSerialPort;
class SerialPort : public ISerialPort
{
    Q_OBJECT

public:
    explicit SerialPort(const SerialSetting &setting, QObject *parent = nullptr);

    virtual ~SerialPort();

    bool open(QIODevice::OpenMode mode);

    void close();

    qint64 write(const QByteArray &data);

    bool isOpen() const;

public slots:
    void onCloseSerialPort();

    bool onOpenSerialPort();

signals:
    void readyRead();

    void closeSerialPort();

    bool openSerialPort();

protected:
    virtual void timerEvent(QTimerEvent* event);

protected:
    int                     mReadTimerId;
    int                     mStatisticsTimerId;
    int                     mVirtualPaketTimerId;
    QSerialPort             *m_pSerialPort = Q_NULLPTR;
    QIODevice::OpenMode     mOpenMode = QIODevice::ReadWrite;

};

#endif // SERIALPORT_H
