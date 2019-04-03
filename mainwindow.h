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
    void on_pushButton_clicked();
    void on_pushButtonStart_clicked();
    void setData(QByteArray Data);
    void enableNibpDisp(bool bEnabled);
    void enableEcgDisp(bool bEnabled);
    void enableTempDisp(bool bEnabled);
    void enableIbpDisp(bool bEnabled);
    void enableSp02Disp(bool bEnabled);
    void on_pushButton_14_clicked();
    void drawEcgWave();

signals:
    void openThread();

private:
    void initDisp();
    void detectPort();
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
