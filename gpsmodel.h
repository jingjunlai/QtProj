#ifndef GPSMODEL_H
#define GPSMODEL_H

#include "model.h"

class GpsModel : public Model
{
public:
    GpsModel();
    ~GpsModel();

    void handleData(const QByteArray &Data);

public slots:
    void doWork();


};

#endif // GPSMODEL_H
