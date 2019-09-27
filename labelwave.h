#ifndef LABELWAVE_H
#define LABELWAVE_H

#include "module/moduledata.h"

#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QTimer>

#define SCREEN_WIDTH        474                 /*直接宏定义屏幕长度,单位为mm 调用QT的api获取的物理长度为什么是错误值？*/
#define FULL_W_PIX          1920                /*长度像素*/
#define SCREEN_HEIGHT       267                 /*物理高度*/
#define FULL_H_PIX          1080                /*像素高度*/

#define UPDATE_TIMER_CNT    500
#define DRAW_TIMER_CNT      10                  /*波形刷新速率为10ms，行为:读数，把数以点连接起来，刷新画面*/

#define ECG_SAMPLE_RATE     1000                /*ECG采样率为1000/s*/
#define OTHER_SAMPLE_RATE   100                 /*非心电的为100/s*/
#define RESP_SAMPLE_RATE    50                  /*呼吸为50/s  协议怎么写的100/s？ */

#define ECG_READ_CNT        10                  /*10ms刷新一次，1秒就刷新100次，而1秒写1000个点，所以读取速度要1次读10个点*/
#define OTHER_READ_CNT      1                   /*10ms刷新一次，1秒就刷新100次，而1秒写100个点，所以读取速度要1次读1个点*/
#define RESP_READ_CNT       1                   /*待定，和协议不符合*/

class LabelWave : public QLabel
{
    Q_OBJECT

public:

    typedef enum
    {
        Ecg_Start = 0,
        I = Ecg_Start,
        II,
        III,
        AVR,
        AVL,
        AVF,
        V1,
        V2,
        V3,
        V4,
        V5,
        V6,
        Ecg_End = V6,
        Spo2,
        Co2,
        Ibp_Start = 14,
        Ibp1 = Ibp_Start,
        Ibp2,
        Ibp3,
        Ibp4,
        Ibp_End = Ibp4,
        Resp,
        Resp_s,
        Nibp,
        MAX_Wave
    } WaveId;

    typedef enum
    {
        SPEED6_25 = 0,
        SPEED12_5,
        SPEED25,
        SPEED50,
        SPEED100,
        FULL_SPEED,
        Max_SPEED
    } WaveSpeed;

    typedef enum
    {
        GAIN_0_25X = 0,
        GAIN_0_5X,
        GAIN_1X,
        GAIN_2X,
        GAIN_4X,
        MAX_GAIN
    } WaveGain;

    explicit LabelWave(QWidget *parent, int Width, int Height);

    virtual ~LabelWave();

    void setWaveEnabled(bool enabled);

    bool getEnabled() const;

    void setSpeed(const WaveSpeed &speed);

    void reset();

    void reload(int Width, int Height);

    void setGain(const WaveGain &gain);

    void setLimit(int limit);

    void freezeWave();

    void unfreezeWave();

    virtual void setPointer(ModuleData *p) = 0;

    virtual void onlyReadData() = 0;

signals:

public slots:

    virtual void drawProcess() = 0;

protected:

    virtual void drawOtherItem() = 0;

    virtual void drawWaveF(float value);

    virtual void initDisp(int Width, int Height);

    void paintEvent(QPaintEvent *);

    QPixmap             m_Pix;
    QPen                *m_pPen;
    QPen                *m_pBlackPen;
    QPen                *m_pRedPen;
    QPen                *m_pThickPen;
    QPainter            *m_pPainter;
    QPainter            *m_pLabelPainter;

    bool                m_bEnabled = false;
    bool                m_bFrozen = false;

    float               m_fGain = 1.0;          //增益
    int                 m_iLimit = 0;            //画图时Y轴取值的上下限，应该固定，这里先用变量作为可调整

    /*配置速率用到的变量*/
    int                 m_iSampleRate = 0;      //采样率
    int                 m_iPixPerSecond = 0;    //1秒应该要画多少个像素点，由波形速度决定
    double              m_fXRate = 0;           //x轴的x值速率值 = 1.0 / 采样率 / 每秒应该画的像素点

    /*坐标系用到的变量*/
    double              m_fLastX = 0.0;
    double              m_fCurrentX = 0.0;
    double              m_fLastY = 0.0;

};

#endif // LABELWAVE_H
