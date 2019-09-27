#ifndef LABELECGWAVE_H
#define LABELECGWAVE_H

#include "labelwave.h"
#include "module/moduleecg.h"


class LabelEcgText : public QLabel
{


public:
    explicit LabelEcgText(QWidget *parent = 0);


protected:

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    QPixmap             m_Pix;
};

class LabelEcgScale;
class LabelEcgWave : public LabelWave
{
public:
    explicit LabelEcgWave(int ecgCnt, QWidget *parent, int Width, int Height);

    virtual ~LabelEcgWave();

    static void setReadCnt(int cnt);

protected:

    virtual void setPointer(ModuleData *p) Q_DECL_OVERRIDE;

    virtual void onlyReadData() Q_DECL_OVERRIDE;

    virtual void drawProcess() Q_DECL_OVERRIDE;

    virtual void drawOtherItem() Q_DECL_OVERRIDE;

    virtual void readSharedData();

private:

    int                         m_iecgCnt = 0;
    int                         m_iScaleLen = 0;        //标尺长度
    float                       m_fLastGain = 0;
    LabelEcgScale               *m_pScale = Q_NULLPTR;
    LabelEcgText                *m_pText = Q_NULLPTR;
};



#endif // LABELECGWAVE_H
