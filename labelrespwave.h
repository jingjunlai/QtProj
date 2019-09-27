#ifndef LABELRESPWAVE_H
#define LABELRESPWAVE_H

#include "labelwave.h"
#include "module/moduleresp.h"

class LabelRespWave : public LabelWave
{
public:
    explicit LabelRespWave(QWidget *parent, int Width, int Height);

    ~LabelRespWave();


protected:

    virtual void setPointer(ModuleData *p) Q_DECL_OVERRIDE;

    virtual void onlyReadData() Q_DECL_OVERRIDE;

    virtual void drawProcess() Q_DECL_OVERRIDE;

    virtual void drawOtherItem() Q_DECL_OVERRIDE;


private:

    RingBuffer<RespWaveUnit>     *m_pWave = Q_NULLPTR;
    RespStatus                   *m_pStatus = Q_NULLPTR;

};



#endif // LABELRESPWAVE_H
