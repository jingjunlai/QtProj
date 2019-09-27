#ifndef LABELCO2WAVE_H
#define LABELCO2WAVE_H

#include "labelwave.h"
#include "module/moduleco2.h"

class LabelCo2Wave : public LabelWave
{
public:
    explicit LabelCo2Wave(QWidget *parent, int Width, int Height);

    ~LabelCo2Wave();

protected:

    virtual void setPointer(ModuleData *p) Q_DECL_OVERRIDE;

    virtual void onlyReadData() Q_DECL_OVERRIDE;

    virtual void drawProcess() Q_DECL_OVERRIDE;

    virtual void drawOtherItem() Q_DECL_OVERRIDE;


private:

    RingBuffer<CO2WaveUnit>     *m_pWave = Q_NULLPTR;
    CO2Status                   *m_pStatus = Q_NULLPTR;
};



#endif // LABELCO2WAVE_H
