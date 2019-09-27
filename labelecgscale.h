#ifndef LABELECGSCALE_H
#define LABELECGSCALE_H

#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>

#define ScaleWidth      4       //标尺的宽度

class LabelEcgScale : public QLabel
{
    Q_OBJECT
public:
    explicit LabelEcgScale(QWidget *parent = 0, int len = 0);

    virtual ~LabelEcgScale();


signals:

public slots:

protected:

    void paintEvent(QPaintEvent *);

    QPixmap             m_Pix;
    QPainter            *m_pPainter;
    QPainter            *m_pLabelPainter;

};

#endif // LABELECGSCALE_H
