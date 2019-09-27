#include "chartecg.h"

ChartEcg::ChartEcg(QWidget *parent, int Width, int Height)
    : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    this->resize(Width, Height);
    m_pChartView = new QChartView(this);
    m_pChartView->setMinimumSize(0, 0);

    m_pChartView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_pChartView->resize(Width, Height);
    m_pChartView->setStyleSheet("padding:0px;border:0px;");

    //TODO 其实这里可以用默认的坐标轴
    axisY_ECG = new QValueAxis();
    axisX_ECG = new QValueAxis();
    ecgSeries = new QLineSeries();//TODO 考虑改为平滑曲线
    ecgWaveLineChart = new QChart();

    //添加曲线到chart中
    ecgWaveLineChart->addSeries(ecgSeries);

    //设置坐标轴显示范围
    axisY_ECG->setRange(0, 300);
    axisX_ECG->setRange(0, 600);
//    axisX_ECG->setTickCount(30);
//    axisY_ECG->setTickCount(10);

    //设置坐标轴的颜色，粗细和设置网格显示
    axisX_ECG->setGridLinePen(QPen(Qt::red, 1, Qt::DashDotDotLine, Qt::SquareCap, Qt::RoundJoin)); //网格样式
    axisY_ECG->setGridLinePen(QPen(Qt::red, 1, Qt::DashDotDotLine, Qt::SquareCap, Qt::RoundJoin));

    axisX_ECG->setLinePen(QPen(Qt::red, 1, Qt::DashDotDotLine, Qt::SquareCap, Qt::RoundJoin));//坐标轴样式
    axisY_ECG->setLinePen(QPen(Qt::red, 1, Qt::DashDotDotLine, Qt::SquareCap, Qt::RoundJoin));

    axisY_ECG->setGridLineVisible(false);//显示线框
    axisX_ECG->setGridLineVisible(false);

    axisX_ECG->setLabelsVisible(false);//不显示具体数值
    axisY_ECG->setLabelsVisible(false);

    axisY_ECG->setLineVisible(false);
    axisX_ECG->setLineVisible(false);

    axisY_ECG->setVisible(false);
    axisY_ECG->setVisible(false);

    //把坐标轴添加到chart中，第二个参数是设置坐标轴的位置，
    //只有四个选项，下方：Qt::AlignBottom，左边：Qt::AlignLeft，右边：Qt::AlignRight，上方：Qt::AlignTop
    ecgWaveLineChart->addAxis(axisX_ECG, Qt::AlignBottom);
    ecgWaveLineChart->addAxis(axisY_ECG, Qt::AlignLeft);

    //把曲线关联到坐标轴
    ecgSeries->attachAxis(axisX_ECG);
    ecgSeries->attachAxis(axisY_ECG);
    ecgSeries->setColor(QColor(Qt::yellow));//设置线的颜色

//    ecgSeries->setUseOpenGL(true);//openGL加速
//    setLineChartMargins(ecgWaveLineChart, 2);//设置折线图边距

    ecgWaveLineChart->legend()->hide();//不显示注释
    ecgWaveLineChart->layout()->setContentsMargins(0,0,0,0);
    ecgWaveLineChart->setMargins(QMargins(0,0,0,0));
    ecgWaveLineChart->setBackgroundRoundness(0);
    ecgWaveLineChart->setBackgroundBrush(QBrush(QColor(0,0,255)));
    ecgWaveLineChart->setAutoFillBackground(true);
//    m_pChartView->setChart(ecgWaveLineChart);
    m_pChartView->show();
    m_pTimer = new QTimer;
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(drawWave()));

    m_pTimer->start(8);
}

void ChartEcg::drawWave()
{
    static double axis_x = 0;
    int timesCounts = axis_x / 600;//查看数据是否超过了600
    if (timesCounts > 0)//如果第一次界面绘制结束，之后存在了600个点
    {
        axis_x = axis_x - timesCounts * 600;
        ecgPointBuffer[axis_x].setY(axis_x);

    }
    else//如果是第一次界面
    {
        ecgPointBuffer.append(QPointF(axis_x, 10));
    }
    axis_x += 0.1;
    ecgSeries->replace(ecgPointBuffer);

}



//设置chartView的边缘宽度
void ChartEcg::setLineChartMargins(QChart *chart, int margin) {
    QMargins m_Margin;
    m_Margin.setLeft(margin);
    m_Margin.setBottom(margin);
    m_Margin.setRight(margin);
    m_Margin.setTop(margin);
    chart->setMargins(m_Margin);
}
