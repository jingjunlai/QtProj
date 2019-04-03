#include "labelecgwave.h"

LabelEcgWave::LabelEcgWave()
{

}

LabelEcgWave::LabelEcgWave(QWidget *parent, int Width, int Height):
    QLabel(parent)
{
    m_Pix = QPixmap(Width,Height);
    m_Pix.fill(Qt::black);
    m_pPen = new QPen(QColor(0,225,0));
    m_pPen->setWidth(2);
    m_pPen1 = new QPen(QColor(0,0,0));
    m_pPen1->setWidth(2);
    m_pPainter=new QPainter(&m_Pix);
    m_pLabelPainter=new QPainter;
    this->resize(Width,Height);
    show();
}

LabelEcgWave::~LabelEcgWave()
{
    delete m_pLabelPainter;
    delete m_pPainter;
    delete m_pPen;
    delete m_pPen1;
    m_Pix.detach();
}

void LabelEcgWave::drawWave()
{
    static int iLastX = 0, iCurrentX = 0;
    m_pPainter->setPen(*m_pPen);
    m_pPainter->drawLine(iLastX,this->height()/2 ,iCurrentX, this->height()/2);
    m_pPainter->setPen(*m_pPen1);
    m_pPainter->drawRect(iCurrentX+1, 0, 2, this->height());
    iLastX = iCurrentX;
    iCurrentX++;
    if(iCurrentX >= this->width())
    {
        iCurrentX = 0;
    }
}

void LabelEcgWave::paintEvent(QPaintEvent *painter)
{
    m_pLabelPainter->begin(this);
    m_pLabelPainter->drawPixmap((painter->rect()),m_Pix);
    m_pLabelPainter->end();
}

