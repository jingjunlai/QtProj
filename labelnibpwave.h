#ifndef LABELNIBPWAVE_H
#define LABELNIBPWAVE_H


#include "labelwave.h"
#include "module/modulenibp.h"

class LabelNibpWave : public LabelWave
{
public:
    explicit LabelNibpWave(QWidget *parent, int Width, int Height);

    ~LabelNibpWave();


protected:

//    virtual void initDisp(int Width, int Height) Q_DECL_OVERRIDE;

    virtual void setPointer(ModuleData *p) Q_DECL_OVERRIDE;

    virtual void onlyReadData() Q_DECL_OVERRIDE;

    virtual void drawProcess() Q_DECL_OVERRIDE;

    virtual void drawOtherItem() Q_DECL_OVERRIDE;


private:

    RingBuffer<NibpWaveUnit>     *m_pWave = Q_NULLPTR;

};
#endif // LABELNIBPWAVE_H
