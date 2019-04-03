#ifndef LABELECGWAVE_H
#define LABELECGWAVE_H

#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QDebug>


class LabelEcgWave : public QLabel
{
public:
    LabelEcgWave();
    LabelEcgWave(QWidget *parent,int Width,int Height);
    ~LabelEcgWave();
    void drawWave();

signals:

public slots:

protected:
   void paintEvent(QPaintEvent *);

private:
   QPixmap m_Pix;
   QPen *m_pPen;
   QPen *m_pPen1;
   QPainter *m_pPainter;
   QPainter *m_pLabelPainter;
};

#endif // LABELECGWAVE_H
