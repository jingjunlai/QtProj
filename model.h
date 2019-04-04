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
    void sendPortDataSizeToMain(unsigned int uiSize);

protected slots:
    virtual void receiveData(const QByteArray &data) = 0;
    virtual void doWork() = 0;

protected:
    void addPortCnt(unsigned int uiSize);

private:
    MySerialPort *m_pSerialport;
};

#endif // MODEL_H
