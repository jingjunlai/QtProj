#include "labelrespwave.h"

#include <math.h>

LabelRespWave::LabelRespWave(QWidget *parent, int Width, int Height)
    : LabelWave(parent, Width, Height)
{

    m_iLimit = 500;
    m_iSampleRate = OTHER_SAMPLE_RATE;
    initDisp(Width, Height);
}

LabelRespWave::~LabelRespWave()
{

}

void LabelRespWave::setPointer(ModuleData *p)
{
    m_pWave = dynamic_cast<RespModuleData *>(p)->m_pWave;
    m_pStatus = dynamic_cast<RespModuleData *>(p)->m_pStatus;
}

void LabelRespWave::onlyReadData()
{

}

void LabelRespWave::drawProcess()
{
    if(m_pWave != Q_NULLPTR)
    {
        drawOtherItem();
        /*读出缓存的值*/
        RespWaveUnit *pw = new RespWaveUnit[RESP_READ_CNT * 2];
        int size = m_pWave->size();
        int len;

        if(size < 200)
            len = m_pWave->read(pw, RESP_READ_CNT);
        else
            len = m_pWave->read(pw, RESP_READ_CNT * 2);
        /*脱落画虚线，长度为0表示读太快了没数据了，画空白*/
        if(len != 0)
        {
            float v;
            for(int i = 0; i < len; i++)
            {
                if(pw[i].bValid)
                {
                    v = ((float)pw[i].wave * this->height() / m_iLimit);
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
        for(int i = 0; i < RESP_READ_CNT; i++)
            drawWaveF(SENSOROFF_DATA);
    }
    update();
}

void LabelRespWave::drawOtherItem()
{

}


