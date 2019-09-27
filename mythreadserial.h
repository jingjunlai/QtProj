#ifndef MYTHREADSERIAL_H
#define MYTHREADSERIAL_H

#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include "qringbuffer_p.h"

class MySerialPort : public QObject
{
    Q_OBJECT
public:
    explicit MySerialPort(QObject *parent = NULL);
    MySerialPort(const QString Port, const int &iBuadRate, QObject *parent = NULL);
    ~MySerialPort();

    static void detectPort(QStringList *slt);

    //QRingBuffer *m_pRingbuffer;

public slots:
    void readSerialData();

    void doWork();

    void testReceiveData();
signals:

    void sendDataToModel(QByteArray Data);

    void enableMainWork(int id);

private:

    QTimer          *m_pTimer;
    QSerialPort     *m_pSerial;
    QString         m_Port;
    QMutex          m_Mutex;
    int             m_iBuadRate;
};

#endif // MYTHREADSERIAL_H
