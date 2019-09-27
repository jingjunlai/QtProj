#include "labelecgscale.h"

LabelEcgScale::LabelEcgScale(QWidget *parent, int len)
    : QLabel(parent)
{
    m_Pix = QPixmap(ScaleWidth, len);
    m_Pix.fill(Qt::yellow);
    m_pPainter=new QPainter(&m_Pix);
    m_pLabelPainter=new QPainter;
    this->resize(ScaleWidth, len);
    show();
}

LabelEcgScale::~LabelEcgScale()
{
    delete m_pLabelPainter;
    delete m_pPainter;
}


void LabelEcgScale::paintEvent(QPaintEvent *painter)
{
    m_pLabelPainter->begin(this);
    m_pLabelPainter->drawPixmap((painter->rect()),m_Pix);
    m_pLabelPainter->end();
}
