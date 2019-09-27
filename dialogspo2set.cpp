#include "dialogspo2set.h"
#include "ui_dialogspo2set.h"
#include "mainwindow.h"

DialogSPO2Set::DialogSPO2Set(QWidget *parent) :
    IDialogSet(parent),
    ui(new Ui::DialogSPO2Set)
{
    ui->setupUi(this);
}

DialogSPO2Set::~DialogSPO2Set()
{
    delete ui;
}

void DialogSPO2Set::setSpo2WaveSpeed(const LabelWave::WaveSpeed &speed)
{
    ui->comboBoxWaveSpeedSpo2->setCurrentIndex((int)speed);
}

void DialogSPO2Set::setRespWaveSpeed(const LabelWave::WaveSpeed &speed)
{
    ui->comboBoxWaveSpeedResp->setCurrentIndex((int)speed);
}

void DialogSPO2Set::on_buttonBox_accepted()
{
    if(m_WaveList[Spo2] != Q_NULLPTR)
        m_WaveList[Spo2]->setSpeed((LabelWave::WaveSpeed)ui->comboBoxWaveSpeedSpo2->currentIndex());
    if(m_WaveList[Resp] != Q_NULLPTR)
    m_WaveList[Resp]->setSpeed((LabelWave::WaveSpeed)ui->comboBoxWaveSpeedResp->currentIndex());

    emit setMainConfig(LABEL_SPO2, MainWindow::SetSpeed, ui->comboBoxWaveSpeedSpo2->currentIndex());
    emit setMainConfig(LABEL_RESP_S, MainWindow::SetSpeed, ui->comboBoxWaveSpeedResp->currentIndex());
}
