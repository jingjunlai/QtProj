#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialogecgset.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include "mythreadserial.h"
#include "model.h"
#include "bm100amodel.h"

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

private:
//    void initNibpDisp();
//    void initEcgDisp();
//    void initTempDisp();
//    void initSpo2Disp();
//    void initIbpDisp();



    Ui::MainWindow *ui;
    MyThreadSerial *m_Serialport;
    Model *m_Model;
    unsigned long m_ulDataNum;
};

#endif // MAINWINDOW_H
