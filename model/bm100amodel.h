#ifndef BM100AMODEL_H
#define BM100AMODEL_H

#include "model.h"

class BM100AModel //: public Model
{
public:
    BM100AModel();
    ~BM100AModel();
    void receiveData(const QByteArray &Data);

protected:
    void unpackData(unsigned char *pucBuff);

public slots:
    void doWork();
};

#endif // BM100AMODEL_H
