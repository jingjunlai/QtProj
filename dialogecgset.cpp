#include "dialogecgset.h"
#include "ui_dialogecgset.h"
#include "net/mytcpserver.h"
#include "mainwindow.h"
#include <QDebug>

DialogECGSet::DialogECGSet(QWidget *parent) :
    IDialogSet(parent),
    ui(new Ui::DialogECGSet)
{
    ui->setupUi(this);
    m_pButtonGroupGain = new QButtonGroup(this);
    m_pButtonGroupGain->addButton(ui->radioButton0_25X, 0);
    m_pButtonGroupGain->addButton(ui->radioButton0_5X, 1);
    m_pButtonGroupGain->addButton(ui->radioButton1X, 2);
    m_pButtonGroupGain->addButton(ui->radioButton2X, 3);
    m_pButtonGroupGain->addButton(ui->radioButton4X, 4);
    ui->radioButton1X->setChecked(true);

    m_pButtonGroupMode = new QButtonGroup(this);
    m_pButtonGroupMode->addButton(ui->radioButtonMon, 0);
    m_pButtonGroupMode->addButton(ui->radioButtonOpe, 1);
    m_pButtonGroupMode->addButton(ui->radioButtonDiag, 2);
    m_pButtonGroupMode->addButton(ui->radioButtonST, 3);
    ui->radioButtonMon->setChecked(true);
}

DialogECGSet::~DialogECGSet()
{
    delete ui;
}

void DialogECGSet::setWaveSpeed(const LabelWave::WaveSpeed &speed)
{
    ui->comboBoxWaveSpeed->setCurrentIndex((int)speed);
}

void DialogECGSet::on_buttonBox_accepted()
{
    if(m_iClientId == INVALID_DATA)
    {
        qDebug() << "Client is not be connected!";
        return;
    }

    if(m_pTcpServer == Q_NULLPTR)
    {
        qDebug() << "Tcp Server is null";
        return;
    }

    for(int i = 0; i != m_WaveList.size(); i++)
    {
        m_WaveList[i]->setGain((LabelWave::WaveGain)m_pButtonGroupGain->checkedId());
        m_WaveList[i]->setSpeed((LabelWave::WaveSpeed)ui->comboBoxWaveSpeed->currentIndex());
    }

    emit setMainConfig(LABEL_ECG, MainWindow::SetGain, m_pButtonGroupGain->checkedId());
    emit setMainConfig(LABEL_ECG, MainWindow::SetSpeed, ui->comboBoxWaveSpeed->currentIndex());

    EcgConfigField d;

    d.leadType = (CmsEcgLeadType)ui->comboBoxLeadType->currentIndex();
    d.monitorLead = (CmsECGLeadIndex)ui->comboBoxCalLead->currentIndex();
    d.workMode = (CmsEcgWorkMode)m_pButtonGroupMode->checkedId();

    auto configData = new CmsEcgConfigData(CMS_ModuleEcg, CMS_LabelEcg);
    configData->setConfig(d);

    QSharedPointer<ICmsData> data(configData);

    emit sendConfigData(m_iClientId, data);

    data.clear();
}
