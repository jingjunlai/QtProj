#include "dialogrespset.h"
#include "ui_dialogrespset.h"
#include "net/cmsrespconfigdata.h"
#include "labelwave.h"
#include "mainwindow.h"

DialogRespSet::DialogRespSet(QWidget *parent) :
    IDialogSet(parent),
    ui(new Ui::DialogRESPSet)
{
    ui->setupUi(this);

    m_pButtonGroupGain = new QButtonGroup(this);
    m_pButtonGroupGain->addButton(ui->radioButton0_25X, 0);
    m_pButtonGroupGain->addButton(ui->radioButton1X, 2);
    m_pButtonGroupGain->addButton(ui->radioButton2X, 3);
    m_pButtonGroupGain->addButton(ui->radioButton4X, 4);
    ui->radioButton1X->setChecked(true);

    m_pButtonGroupLead = new QButtonGroup(this);
    m_pButtonGroupLead->addButton(ui->radioButtonRA_LL, 0);
    m_pButtonGroupLead->addButton(ui->radioButtonRA_LA, 1);
    m_pButtonGroupLead->addButton(ui->radioButtonLA_RL, 2);
    m_pButtonGroupLead->addButton(ui->radioButtonRL_LL, 3);
    ui->radioButtonRA_LL->setChecked(true);
}

DialogRespSet::~DialogRespSet()
{
    delete ui;
}

void DialogRespSet::setWaveSpeed(const LabelWave::WaveSpeed &speed)
{
    ui->comboBoxWaveSpeed->setCurrentIndex((int)speed);
}

void DialogRespSet::on_buttonBox_accepted()
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
    emit setMainConfig(LABEL_RESP, MainWindow::SetSpeed, ui->comboBoxWaveSpeed->currentIndex());

    RespConfigField d;

    auto configData = new CmsRespConfigData(CMS_ModuleResp, CMS_LabelResp);
    configData->setConfig(d);

    QSharedPointer<ICmsData> data(configData);

    emit sendConfigData(m_iClientId, data);

    data.clear();
}
