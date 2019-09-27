#include "labelwave.h"
#include <math.h>

LabelWave::LabelWave(QWidget *parent, int Width, int Height)
    : QLabel(parent)
{

    Q_UNUSED(Width);
    Q_UNUSED(Height);
//    initDisp(Width,Height);
}

LabelWave::~LabelWave()
{
    delete m_pLabelPainter;
    delete m_pPainter;
    delete m_pPen;
    delete m_pBlackPen;
    delete m_pThickPen;
    delete m_pRedPen;
    m_Pix.detach();
}

void LabelWave::initDisp(int Width, int Height)
{
    /*画布*/
    m_Pix = QPixmap(Width,Height);
    m_Pix.fill(Qt::black);
    /*画波形的笔*/
    m_pPen = new QPen(QColor(0,225,0));
    m_pPen->setWidth(2);
    /*画黑区域的笔*/
    m_pBlackPen = new QPen(QColor(0,0,0));
    m_pBlackPen->setWidth(2);
    /*画粗线的笔*/
    m_pThickPen = new QPen(QColor(0,225,0));
    m_pThickPen->setWidth(4);
    /*红色的笔，调试用*/
    m_pRedPen = new QPen(QColor(0,255,0));
    m_pRedPen->setStyle(Qt::DashDotLine);
    m_pRedPen->setWidth(2);
    /*在画布上作画的painter*/
    m_pPainter=new QPainter;//(&m_Pix);
    /*把整个画布画在parent上的painter*/
    m_pLabelPainter=new QPainter;

    this->resize(Width,Height);
}

void LabelWave::setWaveEnabled(bool enabled)
{
    (m_bEnabled = enabled) == true? show() : hide();
}

bool LabelWave::getEnabled() const
{
    return m_bEnabled;
}

void LabelWave::reset()
{
    m_fLastX = 0.0;
    m_fCurrentX = 0.0;
    m_fLastY = 0.0;
    m_Pix.detach();
}

void LabelWave::reload(int Width, int Height)
{
    this->resize(Width, Height);
    m_Pix = m_Pix.scaled(QSize(Width, Height));
    m_Pix.fill(Qt::red);
    reset();
}

void LabelWave::setGain(const LabelWave::WaveGain &gain)
{
    switch(gain)
    {
        default:
        case GAIN_1X : m_fGain = 1; break;
        case GAIN_0_25X : m_fGain = 0.25; break;
        case GAIN_0_5X : m_fGain = 0.5; break;
        case GAIN_2X : m_fGain = 2; break;
        case GAIN_4X : m_fGain = 4; break;
    }
}

void LabelWave::setLimit(int limit)
{
    m_iLimit = limit;
}

void LabelWave::freezeWave()
{
    m_bFrozen = true;
}

void LabelWave::unfreezeWave()
{
    m_bFrozen = false;
}

void LabelWave::setSpeed(const WaveSpeed &speed)
{
    switch(speed)
    {
        default:
        case SPEED6_25 : m_iPixPerSecond = 25; break;
        case SPEED12_5 : m_iPixPerSecond = 50; break;
        case SPEED25 : m_iPixPerSecond = 100; break;
        case SPEED50 : m_iPixPerSecond = 200; break;
        case SPEED100 : m_iPixPerSecond = 400; break;
        case FULL_SPEED: m_iPixPerSecond = m_iSampleRate; break;
    }
    m_fXRate = 1.0 / m_iSampleRate * m_iPixPerSecond;
}

void LabelWave::drawWaveF(float value)
{
    if(m_bFrozen)
        return;

    m_pPainter->begin(&m_Pix);

    if(value == SENSOROFF_DATA)
    {
        m_fCurrentX += m_fXRate;
        m_pPainter->setPen(*m_pPen);
        if((int)m_fCurrentX % 6 == 0)
            m_pPainter->drawLine(m_fLastX,this->height()/2 ,m_fCurrentX, this->height()/2);
        m_fLastX = m_fCurrentX;
    }
    else
    {
        m_fCurrentX += m_fXRate;
        m_pPainter->setPen(*m_pPen);
        QPointF curr(m_fCurrentX, this->height()/2 - value);
        QPointF last(m_fLastX, this->height()/2 - m_fLastY);
        m_pPainter->drawLine(last, curr);
        m_fLastY = value;
        m_fLastX = m_fCurrentX;
    }
    m_pPainter->setPen(*m_pBlackPen);
    m_pPainter->fillRect(m_fCurrentX+3, 0, 4, this->height(), Qt::black);

    if(m_fCurrentX >= this->width())
    {
        m_pPainter->fillRect(0, 0, 5, this->height(), Qt::black);
        m_fCurrentX = 0;
        m_fLastX = 0;
    }

    m_pPainter->end();
}

void LabelWave::paintEvent(QPaintEvent *painter)
{
    Q_UNUSED(painter);
    /*把整个画布画出来*/
    m_pLabelPainter->begin(this);
    m_pLabelPainter->drawPixmap((m_Pix.rect()),m_Pix);
    m_pLabelPainter->end();
}

