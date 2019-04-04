#ifndef MYTHREADSERIAL_H
#define MYTHREADSERIAL_H

#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QThread>

class MySerialPort : public QObject
{
    Q_OBJECT
public:
    explicit MySerialPort(QObject *parent = NULL);
    MySerialPort(const QString Port, const int &iBuadRate, QObject *parent = NULL);
    ~MySerialPort();

    static void detectPort(QStringList *slt);


public slots:
    void readSerialData();
    void doWork();

signals:
    void sendDataToModel(QByteArray Data);
    void enableMainWork();

private:

    QSerialPort *m_Serial;
    QString m_Port;
    int m_iBuadRate;
};

#endif // MYTHREADSERIAL_H
