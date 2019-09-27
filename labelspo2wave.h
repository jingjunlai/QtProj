#ifndef LABELSPO2WAVE_H
#define LABELSPO2WAVE_H

#include "labelwave.h"
#include "module/modulespo2.h"

class LabelSpo2Wave : public LabelWave
{
public:
    explicit LabelSpo2Wave(QWidget *parent, int Width, int Height);

    ~LabelSpo2Wave();


protected:

    virtual void setPointer(ModuleData *p) Q_DECL_OVERRIDE;

    virtual void onlyReadData() Q_DECL_OVERRIDE;

    virtual void drawProcess() Q_DECL_OVERRIDE;

    virtual void drawOtherItem() Q_DECL_OVERRIDE;


private:

    RingBuffer<Spo2WaveUnit>     *m_pWave = Q_NULLPTR;
    Spo2Status                   *m_pStatus = Q_NULLPTR;

};



#endif // LABELSPO2WAVE_H
