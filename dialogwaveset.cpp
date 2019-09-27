#include "dialogwaveset.h"
#include "ui_dialogwaveset.h"
#include "mainwindow.h"

DialogWaveSet::DialogWaveSet(QWidget *parent) :
    IDialogSet(parent),
    ui(new Ui::DialogWaveSet)
{
    ui->setupUi(this);

    connect(this, SIGNAL(setWaveLead(DialogWaveSet::LeadMode,quint32)), m_mainwindow, SLOT(changeWaveLead(DialogWaveSet::LeadMode,quint32)));

    m_pButtonGroupLead = new QButtonGroup(this);
    m_pButtonGroupLead->setExclusive(false);    //不互斥
    m_pButtonGroupLead->addButton(ui->checkBoxI, LabelWave::I);
    m_pButtonGroupLead->addButton(ui->checkBoxII, LabelWave::II);
    m_pButtonGroupLead->addButton(ui->checkBoxIII, LabelWave::III);
    m_pButtonGroupLead->addButton(ui->checkBoxAVR, LabelWave::AVR);
    m_pButtonGroupLead->addButton(ui->checkBoxAVL, LabelWave::AVL);
    m_pButtonGroupLead->addButton(ui->checkBoxAVF, LabelWave::AVF);
    m_pButtonGroupLead->addButton(ui->checkBoxV1, LabelWave::V1);
    m_pButtonGroupLead->addButton(ui->checkBoxV2, LabelWave::V2);
    m_pButtonGroupLead->addButton(ui->checkBoxV3, LabelWave::V3);
    m_pButtonGroupLead->addButton(ui->checkBoxV4, LabelWave::V4);
    m_pButtonGroupLead->addButton(ui->checkBoxV5, LabelWave::V5);
    m_pButtonGroupLead->addButton(ui->checkBoxV6, LabelWave::V6);
    m_pButtonGroupLead->addButton(ui->checkBoxSpo2, LabelWave::Spo2);
    m_pButtonGroupLead->addButton(ui->checkBoxResp, LabelWave::Resp);
    m_pButtonGroupLead->addButton(ui->checkBoxIbp1, LabelWave::Ibp1);
    m_pButtonGroupLead->addButton(ui->checkBoxIbp2, LabelWave::Ibp2);
    m_pButtonGroupLead->addButton(ui->checkBoxIbp3, LabelWave::Ibp3);
    m_pButtonGroupLead->addButton(ui->checkBoxIbp4, LabelWave::Ibp4);
    m_pButtonGroupLead->addButton(ui->checkBoxRespS, LabelWave::Resp_s);
    m_pButtonGroupLead->addButton(ui->checkBoxCo2, LabelWave::Co2);

    ui->checkBoxI->setChecked(true);
    ui->checkBoxII->setChecked(true);
    ui->checkBoxV1->setChecked(true);
    ui->checkBoxV2->setChecked(true);
    ui->checkBoxSpo2->setChecked(true);
    ui->checkBoxResp->setChecked(true);
    ui->checkBoxIbp1->setChecked(true);
    ui->checkBoxIbp2->setChecked(true);
    ui->checkBoxIbp3->setChecked(true);
    ui->checkBoxIbp4->setChecked(true);
    ui->checkBoxRespS->setChecked(true);
    ui->checkBoxCo2->setChecked(true);
}

DialogWaveSet::~DialogWaveSet()
{
    delete ui;
}

void DialogWaveSet::setWaveLead(DialogWaveSet::LeadMode mode)
{
    ui->comboBoxLeadSet->setCurrentIndex(mode);
}

void DialogWaveSet::on_buttonBox_accepted()
{
    quint32 param = 0xFF;
    auto leadMode = (LeadMode)ui->comboBoxLeadSet->currentIndex();
    if(leadMode == Custom)
    {
        param = m_pButtonGroupLead->checkedId();
    }

    emit setWaveLead(leadMode, param);
}

void DialogWaveSet::on_comboBoxLeadSet_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0:
            {
                m_pButtonGroupLead->setExclusive(false);    //不互斥

                ui->groupBox->setEnabled(false);

                ui->checkBoxSpo2->setEnabled(false);
                ui->checkBoxResp->setEnabled(false);
                ui->checkBoxIbp1->setEnabled(false);
                ui->checkBoxIbp2->setEnabled(false);
                ui->checkBoxIbp3->setEnabled(false);
                ui->checkBoxIbp4->setEnabled(false);
                ui->checkBoxRespS->setEnabled(false);
                ui->checkBoxCo2->setEnabled(false);

                ui->checkBoxI->setChecked(true);
                ui->checkBoxII->setChecked(true);
                ui->checkBoxIII->setChecked(false);
                ui->checkBoxAVR->setChecked(false);
                ui->checkBoxAVL->setChecked(false);
                ui->checkBoxAVF->setChecked(false);
                ui->checkBoxV1->setChecked(true);
                ui->checkBoxV2->setChecked(true);
                ui->checkBoxV3->setChecked(false);
                ui->checkBoxV4->setChecked(false);
                ui->checkBoxV5->setChecked(false);
                ui->checkBoxV6->setChecked(false);

                ui->checkBoxSpo2->setChecked(true);
                ui->checkBoxResp->setChecked(true);
                ui->checkBoxIbp1->setChecked(true);
                ui->checkBoxIbp2->setChecked(true);
                ui->checkBoxIbp3->setChecked(true);
                ui->checkBoxIbp4->setChecked(true);
                ui->checkBoxRespS->setChecked(true);
                ui->checkBoxCo2->setChecked(true);
            }
            break;
        case 1:
            {
                m_pButtonGroupLead->setExclusive(false);    //不互斥

                ui->groupBox->setEnabled(false);

                ui->checkBoxSpo2->setEnabled(false);
                ui->checkBoxResp->setEnabled(false);
                ui->checkBoxIbp1->setEnabled(false);
                ui->checkBoxIbp2->setEnabled(false);
                ui->checkBoxIbp3->setEnabled(false);
                ui->checkBoxIbp4->setEnabled(false);
                ui->checkBoxRespS->setEnabled(false);
                ui->checkBoxCo2->setEnabled(false);

                ui->checkBoxI->setChecked(true);
                ui->checkBoxII->setChecked(true);
                ui->checkBoxIII->setChecked(true);
                ui->checkBoxAVR->setChecked(true);
                ui->checkBoxAVL->setChecked(true);
                ui->checkBoxAVF->setChecked(true);
                ui->checkBoxV1->setChecked(true);
                ui->checkBoxV2->setChecked(true);
                ui->checkBoxV3->setChecked(true);
                ui->checkBoxV4->setChecked(true);
                ui->checkBoxV5->setChecked(true);
                ui->checkBoxV6->setChecked(true);

                ui->checkBoxSpo2->setChecked(false);
                ui->checkBoxResp->setChecked(false);
                ui->checkBoxIbp1->setChecked(false);
                ui->checkBoxIbp2->setChecked(false);
                ui->checkBoxIbp3->setChecked(false);
                ui->checkBoxIbp4->setChecked(false);
                ui->checkBoxRespS->setChecked(false);
                ui->checkBoxCo2->setChecked(false);
            }
            break;
        case 2:
            {
                m_pButtonGroupLead->setExclusive(true);    //互斥

                ui->groupBox->setEnabled(true);


                ui->checkBoxSpo2->setEnabled(true);
                ui->checkBoxResp->setEnabled(true);
                ui->checkBoxIbp1->setEnabled(true);
                ui->checkBoxIbp2->setEnabled(true);
                ui->checkBoxIbp3->setEnabled(true);
                ui->checkBoxIbp4->setEnabled(true);
                ui->checkBoxRespS->setEnabled(true);
                ui->checkBoxCo2->setEnabled(true);

                for(int i = 0; i != m_pButtonGroupLead->buttons().size(); i++)
                {
                    m_pButtonGroupLead->button(i)->setChecked(false);
                }
            }
            break;
    }
}
