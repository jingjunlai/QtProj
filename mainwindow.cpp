#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_ulDataNum(0)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("QMainWindow{background:black;}");

    initDisp();
    /*检测串口*/
    detectPort();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enableWork()
{
    ui->comboBoxModuleType->setEnabled(false);
    ui->comboBoxComPort->setEnabled(false);
    ui->comboBoxBaudrate->setEnabled(false);
    ui->pushButtonStart->setText(QString("Stop"));

    /*启动定时器(TEST)*/
    m_pTimer = new QTimer();
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(drawEcgWave()));
    m_pEcgWave = new LabelEcgWave(ui->groupBoxWave_1, 640, 160);
    m_pEcgWave->move(0,0);
    m_pEcgWave->lower();
    ui->label_2->raise();
    m_pTimer->start(8);

    /*打印串口接收数量*/
    connect(m_pModel, SIGNAL(sendPortDataSizeToMain(uint)), ui->labelPortCnt, SLOT(setText(QString.number(uint))));
}

void MainWindow::disableWork()
{
    m_pTimer->stop();
    m_pEcgWave->deleteLater();
    destroyPort();
    destroyModel();
    closeThread(m_pPortThread);
    closeThread(m_pModelThread);
    ui->comboBoxModuleType->setEnabled(true);
    ui->comboBoxComPort->setEnabled(true);
    ui->comboBoxBaudrate->setEnabled(true);
    ui->pushButtonStart->setText(QString("Start"));
}

void MainWindow::on_pushButton_clicked()
{
    DialogECGSet DialogEcgSet1;
    DialogEcgSet1.exec();
}

void MainWindow::on_pushButtonStart_clicked()
{
    if(!detectPort())
    {
        QMessageBox::about(NULL, "提示", "串口打开失败");
        return;
    }
    if(ui->pushButtonStart->text() == QString("Start"))
    {
        initPort();
        initModel();
        /*启动线程*/
        emit startThread();
    }
    else
    {
        disableWork();
    }
    qDebug()<<"main thread ID = "<<QThread::currentThreadId();
}

void MainWindow::setData(QByteArray Data)
{
    m_ulDataNum += Data.length();
    //ui->textBrowser->append(Data.toHex());
    //ui->labelDataNumValue->setText(QString::number(m_ulDataNum));
}

void MainWindow::enableNibpDisp(bool bEnabled)
{
    ui->groupBoxPara_4->setEnabled(bEnabled);
}

void MainWindow::enableEcgDisp(bool bEnabled)
{
    bEnabled = bEnabled;
}

void MainWindow::enableTempDisp(bool bEnabled)
{
    bEnabled = bEnabled;
}

void MainWindow::enableIbpDisp(bool bEnabled)
{
    bEnabled = bEnabled;
}

void MainWindow::enableSp02Disp(bool bEnabled)
{
    bEnabled = bEnabled;
}


void MainWindow::on_pushButton_14_clicked()
{
    exit(0);
}

void MainWindow::drawEcgWave()
{
    m_pEcgWave->drawWave();
    m_pEcgWave->update();
}


void MainWindow::initDisp()
{
    ui->label_2->setStyleSheet("color:yellow;");
    ui->label_3->setStyleSheet("color:yellow;");
    ui->label_15->setStyleSheet("color:yellow;");
    ui->label_16->setStyleSheet("color:yellow;");
    ui->label_17->setStyleSheet("color:yellow;");
    ui->label_18->setStyleSheet("color:yellow;");
    ui->label_19->setStyleSheet("color:yellow;");
    ui->label_20->setStyleSheet("color:yellow;");
    ui->label_21->setStyleSheet("color:yellow;");
    ui->label_22->setStyleSheet("color:yellow;");
    ui->label_23->setStyleSheet("color:yellow;");
    ui->label_24->setStyleSheet("color:yellow;");
    ui->labelModuleType->setStyleSheet("color:yellow;");
    ui->labelComPort->setStyleSheet("color:yellow;");
    ui->labelBaudrate->setStyleSheet("color:yellow;");
    ui->groupBoxCommand->setStyleSheet("QGroupBox{color:yellow;}");
    ui->groupBoxSelect->setStyleSheet("QGroupBox{color:yellow;}");
    ui->groupBoxPara_1->setStyleSheet("color:yellow;");
    ui->groupBoxPara_2->setStyleSheet("color:yellow;");
    ui->groupBoxPara_3->setStyleSheet("color:yellow;");
    ui->groupBoxPara_4->setStyleSheet("color:yellow;");
    ui->groupBoxPara_5->setStyleSheet("color:yellow;");
    ui->groupBoxPara_6->setStyleSheet("color:yellow;");
    ui->groupBoxPara_7->setStyleSheet("color:yellow;");
    ui->groupBoxPara_8->setStyleSheet("color:yellow;");
    ui->groupBoxPara_9->setStyleSheet("color:yellow;");
    ui->groupBoxPara_10->setStyleSheet("color:yellow;");
    ui->groupBoxPara_11->setStyleSheet("color:yellow;");
    ui->groupBoxPara_12->setStyleSheet("color:yellow;");
}

void MainWindow::initPort()
{
    /*串口线程*/
    m_pSerialport = new MySerialPort(ui->comboBoxComPort->currentText(), ui->comboBoxBaudrate->currentText().toInt());
    m_pPortThread = new QThread(this);
    m_pSerialport->moveToThread(m_pPortThread);
    connect(this, SIGNAL(startThread()), m_pSerialport, SLOT(doWork()));
    connect(m_pSerialport, SIGNAL(enableMainWork()), this, SLOT(enableWork()));
    m_pPortThread->start();
}

void MainWindow::destroyPort()
{
    disconnect(this, SIGNAL(startThread()), m_pSerialport, SLOT(doWork()));
    disconnect(m_pSerialport, SIGNAL(enableMainWork()), this, SLOT(enableWork()));
    m_pSerialport->deleteLater();
    m_pSerialport = NULL;

}


void MainWindow::initModel()
{
    /*模型线程*/
    m_pModel = new GpsModel();
    m_pModel->setPort(m_pSerialport);
    m_pModelThread = new QThread(this);
    m_pModel->moveToThread(m_pModelThread);
    connect(this, SIGNAL(startThread()), m_pModel, SLOT(doWork()));
    m_pModelThread->start();
}

void MainWindow::destroyModel()
{
    m_pModel->deleteLater();
    m_pModel = NULL;
}

bool MainWindow::detectPort()
{
    QStringList slt;
    MySerialPort::detectPort(&slt);
    if(slt.count() == 0)
    {
        ui->pushButtonStart->setEnabled(false);
        return false;
    }
    else
    {
        foreach (const QString &s, slt) {
            ui->comboBoxComPort->addItem(s);
        }
        return true;
    }
}

void MainWindow::closeThread(QThread * const pThread)
{
    pThread->quit();
    pThread->wait();
    pThread->deleteLater();
}

