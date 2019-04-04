#ifndef GPSMODEL_H
#define GPSMODEL_H

#include "model.h"

const int RECEIVE_SIZE = 255;

class GpsModel : public Model
{
public:
    GpsModel();
    ~GpsModel();
    void receiveData(const QByteArray &data) Q_DECL_OVERRIDE;
    void unpackData(unsigned char *pucBuff) Q_DECL_OVERRIDE;

signals:
    void sendPortDataSizeToMain(unsigned int uiSize) Q_DECL_OVERRIDE;

public slots:
    void doWork() Q_DECL_OVERRIDE ;

};

#endif // GPSMODEL_H
