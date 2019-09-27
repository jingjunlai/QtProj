#ifndef GPSMODEL_H
#define GPSMODEL_H

#include <QMutex>
#include "model.h"

const int RECEIVE_SIZE = 255;

class GpsModel : public Model
{
    Q_OBJECT
public:
    GpsModel(QObject *parent = nullptr);

    ~GpsModel();

protected:

    void doWork() Q_DECL_OVERRIDE ;

    void receiveData() Q_DECL_OVERRIDE;

    //void receiveData(const QByteArray &data) Q_DECL_OVERRIDE;

    void handleData(const QByteArray &data) Q_DECL_OVERRIDE;

    void handleSocketData(const QByteArray &data) Q_DECL_OVERRIDE;

    void unpackData(unsigned char *pucBuff, unsigned int uiLen);

private:

    QMutex m_Mutex;

    QRingBuffer m_RingBuffer;
};

#endif // GPSMODEL_H
