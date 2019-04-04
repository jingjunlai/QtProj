#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>
#include "dialogecgset.h"
#include "mythreadserial.h"
#include "model.h"
#include "gpsmodel.h"
#include "labelecgwave.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /*!
     * \brief enableWork ： 等待串口打开后启动工作。
     */
    void enableWork();
    /*!
     * \brief disableWork ： 关闭工作。
     */
    void disableWork();
    /*!
     * \brief on_pushButton_clicked ：
     */
    void on_pushButton_clicked();
    /*!
     * \brief on_pushButtonStart_clicked ：
     */
    void on_pushButtonStart_clicked();
    /*!
     * \brief setData :
     * \param Data :
     */
    void setData(QByteArray Data);
    void enableNibpDisp(bool bEnabled);
    void enableEcgDisp(bool bEnabled);
    void enableTempDisp(bool bEnabled);
    void enableIbpDisp(bool bEnabled);
    void enableSp02Disp(bool bEnabled);
    void on_pushButton_14_clicked();
    void drawEcgWave();
    void dispPortCnt(unsigned int uiSize);

signals:
    void startThread();

private:
    void initDisp();
    void initPort();
    void destroyPort();
    void initModel();
    void destroyModel();
    bool detectPort();
    void closeThread(QThread * const pThread);

    Ui::MainWindow  *ui;
    QThread         *m_pPortThread;
    MySerialPort    *m_pSerialport;
    QSerialPort     *m_pSerial;
    QThread         *m_pModelThread;
    Model           *m_pModel;
    QTimer          *m_pTimer;
    LabelEcgWave    *m_pEcgWave;
    unsigned long   m_ulDataNum;
};

#endif // MAINWINDOW_H
