#ifndef GPSMODEL_H
#define GPSMODEL_H

#include "model.h"

const int RECEIVE_SIZE = 255;

class GpsModel : public Model
{
    Q_OBJECT
public:
    GpsModel();
    ~GpsModel();

private:
    void doWork() Q_DECL_OVERRIDE ;
    void receiveData(const QByteArray &data) Q_DECL_OVERRIDE;

private:
    void unpackData(unsigned char *pucBuff);

};

#endif // GPSMODEL_H
