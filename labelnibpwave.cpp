#include "labelnibpwave.h"

quint16 g_NibpWaveValue = INVALID_DATA;

LabelNibpWave::LabelNibpWave(QWidget *parent, int Width, int Height)
    : LabelWave(parent, Width, Height)
{
    m_iLimit = 1500;
    m_iSampleRate = OTHER_SAMPLE_RATE;
    initDisp(Width, Height);
}

LabelNibpWave::~LabelNibpWave()
{
    g_NibpWaveValue = INVALID_DATA;
}

//void LabelNibpWave::initDisp(int Width, int Height)
//{
//    m_Pix = QPixmap(Width,Height);
//    m_Pix.fill(Qt::black);
//    m_pPen = new QPen(QColor(0,225,0));
//    m_pPen->setWidth(3);
//    m_pBlackPen = new QPen(QColor(0,0,0));
//    m_pBlackPen->setWidth(2);
//    m_pPainter=new QPainter(&m_Pix);
//    m_pThickPen = new QPen(QColor(0,225,0));
//    m_pThickPen->setWidth(4);

//    m_pLabelPainter=new QPainter;
//    this->resize(Width,Height);
//}

void LabelNibpWave::setPointer(ModuleData *p)
{
    m_pWave = dynamic_cast<NibpModuleData *>(p)->m_pWave;
}

void LabelNibpWave::onlyReadData()
{

}

void LabelNibpWave::drawProcess()
{
    if(m_pWave != Q_NULLPTR)
    {
        /*读出缓存的值*/
        NibpWaveUnit *pw = new NibpWaveUnit[OTHER_READ_CNT * 2];
        int size = m_pWave->size();
        int len;

        if(size < 200)
            len = m_pWave->read(pw, OTHER_READ_CNT);
        else
            len = m_pWave->read(pw, OTHER_READ_CNT * 2);
        /*脱落画虚线，长度为0表示读太快了没数据了，画空白*/
        if(len != 0)
        {
            if(pw[len - 1].bVaild)
                g_NibpWaveValue = pw[len - 1].wave;
            else
                g_NibpWaveValue = INVALID_DATA;
        }
        delete [] pw;
    }
    else
    {
        g_NibpWaveValue = INVALID_DATA;
    }
}

void LabelNibpWave::drawOtherItem()
{

}


