#include "dialogarrset.h"
#include "ui_dialogarrset.h"
#include "mainwindow.h"

DialogARRSet::DialogARRSet(QWidget *parent) :
    IDialogSet(parent),
    ui(new Ui::DialogARRSet)
{
    ui->setupUi(this);
}

DialogARRSet::~DialogARRSet()
{
    delete ui;
}

void DialogARRSet::setWaveSpeed(const LabelWave::WaveSpeed &speed)
{
    ui->comboBoxWaveSpeed->setCurrentIndex((int)speed);
}

void DialogARRSet::on_buttonBox_accepted()
{
    for(int i = 0; i != m_WaveList.size(); i++)
    {
        m_WaveList[i]->setSpeed((LabelWave::WaveSpeed)ui->comboBoxWaveSpeed->currentIndex());
    }
    emit setMainConfig(LABEL_CO2, MainWindow::SetSpeed, ui->comboBoxWaveSpeed->currentIndex());
}
