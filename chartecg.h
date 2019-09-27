#ifndef CHARTECG_H
#define CHARTECG_H

#include <QWidget>
#include <QtCharts>
#include <QVector>
#include <QTimer>

QT_CHARTS_USE_NAMESPACE

class ChartEcg : public QWidget
{
    Q_OBJECT
public:
    explicit ChartEcg(QWidget *parent = 0, int Width = 0, int Height = 0);


signals:

private slots:

    void drawWave();

private:

    void setLineChartMargins(QChart * chart, int margin);

    QChartView      *m_pChartView = Q_NULLPTR;

    //ECG相关
    QChart          *ecgWaveLineChart;
    QValueAxis      *axisX_ECG;
    QValueAxis      *axisY_ECG;
    QLineSeries     *ecgSeries;//
    QStringList     originList;

    QTimer              *m_pTimer;
    QVector<QPointF>  ecgPointBuffer;

};

#endif // CHARTECG_H
