#ifndef LABELRESPSWAVE_H
#define LABELRESPSWAVE_H

#include "labelwave.h"
#include "labelspo2wave.h"
#include "module/modulespo2.h"

class LabelRespSWave : public LabelWave
{
public:
    explicit LabelRespSWave(QWidget *parent, int Width, int Height);

    ~LabelRespSWave();

protected:

    virtual void setPointer(ModuleData *p) Q_DECL_OVERRIDE;

    virtual void onlyReadData() Q_DECL_OVERRIDE;

    virtual void drawProcess() Q_DECL_OVERRIDE;

    virtual void drawOtherItem() Q_DECL_OVERRIDE;


private:

    RingBuffer<Spo2WaveUnit>     *m_pWave = Q_NULLPTR;
};

#endif // LABELRESPSWAVE_H
