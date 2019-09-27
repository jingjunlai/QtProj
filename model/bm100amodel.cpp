#include "bm100amodel.h"

BM100AModel::BM100AModel()
{

}

BM100AModel::~BM100AModel()
{

}


void BM100AModel::receiveData(const QByteArray &Data)
{
    QByteArray tempData = Data;
    tempData = "0";
}

void BM100AModel::unpackData(unsigned char *pucBuff)
{
    Q_UNUSED(pucBuff);
}

void BM100AModel::doWork()
{

}

