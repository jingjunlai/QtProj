#include "labelspo2wave.h"

#include <math.h>

Spo2WaveUnit            g_spo2wave[OTHER_READ_CNT * 2];
int                     g_waveLen;

LabelSpo2Wave::LabelSpo2Wave(QWidget *parent, int Width, int Height)
    : LabelWave(parent, Width, Height)
{
    m_iLimit = 1500;
    m_iSampleRate = OTHER_SAMPLE_RATE;
    initDisp(Width, Height);
}

LabelSpo2Wave::~LabelSpo2Wave()
{

}

void LabelSpo2Wave::setPointer(ModuleData *p)
{
    m_pWave = dynamic_cast<Spo2ModuleData *>(p)->m_pWave;
    m_pStatus = dynamic_cast<Spo2ModuleData *>(p)->m_pStatus;
}

void LabelSpo2Wave::onlyReadData()
{

}

void LabelSpo2Wave::drawProcess()
{
    if(m_pWave != Q_NULLPTR)
    {
        drawOtherItem();
        /*读出缓存的值*/
        int size = m_pWave->size();
        if(size < 500)
            g_waveLen = m_pWave->read(g_spo2wave, OTHER_READ_CNT);
        else
            g_waveLen = m_pWave->read(g_spo2wave, OTHER_READ_CNT * 2);
        /*脱落画虚线，长度为0表示读太快了没数据了，画空白*/
        if(g_waveLen != 0)
        {
            float v;
            for(int i = 0; i < g_waveLen; i++)
            {
                if(g_spo2wave[i].bValid)
                {
                    v = ((float)g_spo2wave[i].wave * this->height() / m_iLimit - (this->height() / 2));
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

void LabelSpo2Wave::drawOtherItem()
{

}




