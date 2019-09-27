#include "labelco2wave.h"

#include <math.h>

LabelCo2Wave::LabelCo2Wave(QWidget *parent, int Width, int Height)
    : LabelWave(parent, Width, Height)
{
    m_iLimit = 1500;
    m_iSampleRate = OTHER_SAMPLE_RATE;
    initDisp(Width, Height);
}

LabelCo2Wave::~LabelCo2Wave()
{

}

void LabelCo2Wave::setPointer(ModuleData *p)
{
    m_pWave = dynamic_cast<Co2ModuleData *>(p)->m_pWave;
    m_pStatus = dynamic_cast<Co2ModuleData *>(p)->m_pStatus;
}

void LabelCo2Wave::onlyReadData()
{

}

void LabelCo2Wave::drawProcess()
{
    if(m_pWave != Q_NULLPTR)
    {
        drawOtherItem();
        /*读出缓存的值*/
        CO2WaveUnit *pw = new CO2WaveUnit[OTHER_READ_CNT * 2];
        int size = m_pWave->size();
        int len;

        if(size < 200)
            len = m_pWave->read(pw, OTHER_READ_CNT);
        else
            len = m_pWave->read(pw, OTHER_READ_CNT * 2);
        /*脱落画虚线，长度为0表示读太快了没数据了，就不画*/
        if(len != 0)
        {
            float v;
            for(int i = 0; i < len; i++)
            {
                if(pw[i].bVaild)
                {
                    v = ((float)pw[i].waveValue * this->height() / m_iLimit * m_fGain);
                    drawWaveF(v);
                }
                else
                {
                    drawWaveF(SENSOROFF_DATA);
                }
            }
        }
        delete [] pw;
    }
    else
    {
        drawOtherItem();
        for(int i = 0; i< OTHER_READ_CNT; i++)
            drawWaveF(SENSOROFF_DATA);
    }
    update();
}

void LabelCo2Wave::drawOtherItem()
{

}


