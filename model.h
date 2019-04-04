#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QThread>
#include "mythreadserial.h"

class Model //: public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = 0);
    virtual ~Model();
    void setPort(MySerialPort * const pThreadPort);

protected slots:
    virtual void receiveData(const QByteArray &data) = 0;
    virtual void doWork() = 0;

protected:
    virtual void unpackData(unsigned char *pucBuff) = 0;
    virtual void sendPortDataSizeToMain(unsigned int uiSize) = 0;

    MySerialPort *m_pSerialport;
};

#endif // MODEL_H
