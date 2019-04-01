#ifndef MYTHREADSERIAL_H
#define MYTHREADSERIAL_H

#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QMessageBox>
#include <QDebug>


class MyThreadSerial : public QObject
{
    Q_OBJECT
public:
    explicit MyThreadSerial(const QString &m_Port, const int &m_iBuadRate, QObject *parent = NULL);
    ~MyThreadSerial();

public slots:
    void readSerialData();

signals:
    void sendDataToMain(QByteArray Data);

private:
    QThread *m_Thread;
    QSerialPort *m_Serial;
    QString m_Port;
    int m_iBuadRate;
};

#endif // MYTHREADSERIAL_H
