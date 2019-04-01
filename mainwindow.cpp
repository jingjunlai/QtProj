#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_ulDataNum(0)
{
    ui->setupUi(this);

    ui->textBrowser->document()->setMaximumBlockCount(50);
    /*检测串口*/
    foreach (const QSerialPortInfo &Qspinfo, QSerialPortInfo::availablePorts()) {
        ui->comboBoxComPort->addItem(Qspinfo.portName());
    }

    //m_Model = new BM100AModel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    DialogECGSet DialogEcgSet1;

    DialogEcgSet1.exec();
}

void MainWindow::on_pushButtonStart_clicked()
{
    if(ui->pushButtonStart->text() == QString("Start"))
    {
        ui->comboBoxComPort->setEnabled(false);
        ui->comboBoxBaudrate->setEnabled(false);
        ui->pushButtonStart->setText(QString("Stop"));
        m_Serialport = new MyThreadSerial(ui->comboBoxComPort->currentText(), ui->comboBoxBaudrate->currentText().toInt());
        connect(m_Serialport, SIGNAL(sendDataToMain(QByteArray)), this, SLOT(setData(QByteArray)), Qt::QueuedConnection);
    }
    else
    {
        m_Serialport->deleteLater();
        m_Serialport = NULL;
        ui->comboBoxComPort->setEnabled(true);
        ui->comboBoxBaudrate->setEnabled(true);
        ui->pushButtonStart->setText(QString("Start"));
    }
}

void MainWindow::setData(QByteArray Data)
{
    m_ulDataNum += Data.length();
    ui->textBrowser->append(Data.toHex());
    ui->labelDataNumValue->setText(QString::number(m_ulDataNum));
}

void MainWindow::enableNibpDisp(bool bEnabled)
{
    //ui->groupBoxNibp->setEnabled(bEnabled);
}

void MainWindow::enableEcgDisp(bool bEnabled)
{

}

void MainWindow::enableTempDisp(bool bEnabled)
{

}

void MainWindow::enableIbpDisp(bool bEnabled)
{

}

void MainWindow::enableSp02Disp(bool bEnabled)
{

}

