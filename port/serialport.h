#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = 0);

signals:

public slots:
};

#endif // SERIALPORT_H