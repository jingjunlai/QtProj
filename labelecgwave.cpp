#include "labelecgwave.h"
#include "labelecgscale.h"

#include <math.h>

/*多路ECG共享读出来的点*/
static RingBuffer<ECGWaveUnit>      *s_pWave = Q_NULLPTR;
static ECGStatus                    *s_pStatus = Q_NULLPTR;
static ECGWaveUnit                  s_Wave[ECG_READ_CNT * 2];
static int                          s_iReadFinish = 0;
static int                          s_iReadCnt = 4;
static int                          s_iReadLen = 0;
static int                          s_iReachCnt = 0;

LabelEcgText::LabelEcgText(QWidget *parent)
    : QLabel(parent)
{
    this->resize(40, 20);
    show();
}

void LabelEcgText::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QFont f;
    f.setBold(true);
    f.setPointSize(15);
    QPen Pen(QColor(0,225,0));
    Pen.setWidth(4);
    QPainter Painter;


    Painter.begin(this);
    Painter.setFont(f);
    Painter.setPen(Pen);
    Painter.drawText(0, 15, "1mV");
    Painter.end();
}



LabelEcgWave::LabelEcgWave(int ecgCnt, QWidget *parent, int Width, int Height)
    : LabelWave(parent, Width, Height)
{
    /*I ~ V6*/
    m_iecgCnt = ecgCnt;
    m_iLimit = 3700;
    m_iSampleRate = ECG_SAMPLE_RATE;
    m_iScaleLen = 40;
    initDisp(Width, Height);
    m_pScale = new LabelEcgScale(this, m_iScaleLen);
    m_pScale->move(30, (Height - m_iScaleLen) / 2);
    m_pText = new LabelEcgText(this);
    m_pText->move(33, 90);
}

LabelEcgWave::~LabelEcgWave()
{
    s_iReadFinish = 0;
    s_iReadCnt = 4;
    s_iReadLen = 0;
    s_iReachCnt = 0;
    s_pWave = Q_NULLPTR;
}

void LabelEcgWave::setReadCnt(int cnt)
{
    s_iReadCnt = cnt;
}

void LabelEcgWave::setPointer(ModuleData *p)
{
    s_pWave = dynamic_cast<EcgModuleData *>(p)->m_pWave;
    s_pStatus = dynamic_cast<EcgModuleData *>(p)->m_pStatus;
}

void LabelEcgWave::onlyReadData()
{

}

void LabelEcgWave::drawProcess()
{
    if(s_pWave != Q_NULLPTR)
    {
        drawOtherItem();

        readSharedData();

        if((s_iReachCnt == 0) || (m_fCurrentX != 0))
        {
            if(s_iReadLen != 0)
            {
                float v;
                for(int i = 0; i < s_iReadLen; i++)
                {
                    if(s_Wave[i].vaild[m_iecgCnt] == true)
                    {
                        v = ((float)s_Wave[i].ecg[m_iecgCnt] * this->height() / m_iLimit * m_fGain);
                        drawWaveF(v);
                    }
                    else
                    {
                        drawWaveF(SENSOROFF_DATA);
                    }
                    if(m_fCurrentX == 0)
                    {
                        if(s_iReachCnt == 0)
                            s_iReachCnt = s_iReadCnt;
                        s_iReachCnt--;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        drawOtherItem();
        for(int i = 0; i < ECG_READ_CNT; i++)
            drawWaveF(SENSOROFF_DATA);
    }
    update();
}

void LabelEcgWave::drawOtherItem()
{
    if(m_fLastGain != m_fGain)
    {
        m_pScale->resize(ScaleWidth, m_iScaleLen * m_fGain);
        m_pScale->move(30, (this->height() - m_iScaleLen * m_fGain) / 2);
        m_fLastGain = m_fGain;
    }
}

void LabelEcgWave::readSharedData()
{
    if(s_iReadFinish == 0)
    {
        int size = s_pWave->size();
        if(size < 4000)
            s_iReadLen = s_pWave->read(s_Wave, ECG_READ_CNT);
        else
            s_iReadLen = s_pWave->read(s_Wave, ECG_READ_CNT * 2);

        s_iReadFinish = s_iReadCnt;
    }
    s_iReadFinish--;
}




