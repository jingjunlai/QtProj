#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QThread>
#include "mythreadserial.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = 0);
    virtual ~Model();
    void setPort(MySerialPort * const pThreadPort);

signals:

public slots:
    virtual void handleData(const QByteArray &Data) = 0;
    virtual void doWork() = 0;

private:
    MySerialPort *m_pSerialport;
};

#endif // MODEL_H
