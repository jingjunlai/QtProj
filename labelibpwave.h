#ifndef LABELIBPWAVE_H
#define LABELIBPWAVE_H

#include "labelwave.h"
#include "module/moduleibp.h"

class LabelIbpWave : public LabelWave
{
public:
    explicit LabelIbpWave(int ibpCnt, QWidget *parent, int Width, int Height);

    ~LabelIbpWave();

protected:

    virtual void setPointer(ModuleData *p) Q_DECL_OVERRIDE;

    virtual void onlyReadData() Q_DECL_OVERRIDE;

    virtual void drawProcess() Q_DECL_OVERRIDE;

    virtual void drawOtherItem() Q_DECL_OVERRIDE;

private:

    int                         m_iIbpCnt = 0;

    RingBuffer<IbpWaveUnit>     *m_pWave = Q_NULLPTR;
    IbpStatus                   *m_pStatus = Q_NULLPTR;
};



#endif // LABELIBPWAVE_H
