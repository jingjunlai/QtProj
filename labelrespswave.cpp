#include "labelrespswave.h"

#include <math.h>

extern Spo2WaveUnit            g_spo2wave[];
extern int                     g_waveLen;

LabelRespSWave::LabelRespSWave(QWidget *parent, int Width, int Height)
    : LabelWave(parent, Width, Height)
{
    m_iLimit = 1500;
    m_iSampleRate = OTHER_SAMPLE_RATE;
    initDisp(Width, Height);
}

LabelRespSWave::~LabelRespSWave()
{

}

void LabelRespSWave::setPointer(ModuleData *p)
{
    m_pWave = dynamic_cast<Spo2ModuleData *>(p)->m_pWave;
}

void LabelRespSWave::onlyReadData()
{

}

void LabelRespSWave::drawProcess()
{
    if(m_pWave != Q_NULLPTR)
    {
        drawOtherItem();

        /*脱落画虚线，长度为0表示读太快了没数据了，画空白*/
        if(g_waveLen != 0)
        {
            float v;
            for(int i = 0; i < g_waveLen; i++)
            {
                if(g_spo2wave[i].bValid)
                {
                    v = ((float)g_spo2wave[i].respwave * this->height() / m_iLimit * m_fGain);
                    drawWaveF(v);
                }
                else
                {
                    drawWaveF(SENSOROFF_DATA);
                }
            }
        }
    }
    else
    {
        drawOtherItem();
        for(int i = 0; i < OTHER_READ_CNT; i++)
            drawWaveF(SENSOROFF_DATA);

    }
    update();
}

void LabelRespSWave::drawOtherItem()
{

}






