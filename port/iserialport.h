#ifndef ISERIALPORT_H
#define ISERIALPORT_H

#include <QObject>

class ISerialPort : public QObject
{
    Q_OBJECT
public:
    explicit ISerialPort(QObject *parent = 0);

signals:

public slots:
};

#endif // ISERIALPORT_H