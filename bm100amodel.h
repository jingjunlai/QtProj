#ifndef BM100AMODEL_H
#define BM100AMODEL_H

#include "model.h"

class BM100AModel : public Model
{
public:
    BM100AModel();
    ~BM100AModel();
    void reciveData(const QString &str);
};

#endif // BM100AMODEL_H
