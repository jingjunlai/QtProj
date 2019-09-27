#include "dialogibpset.h"
#include "ui_dialogibpset.h"
#include "net/cmsibpconfigdata.h"
#include "net/cmsibpcommand.h"
#include "mainwindow.h"


DialogIBPSet::DialogIBPSet(QWidget *parent) :
    IDialogSet(parent),
    ui(new Ui::DialogIBPSet)
{
    ui->setupUi(this);
    qRegisterMetaType<QSharedPointer<ICmsData>>("QSharedPointer<ICmsData>");
    connect(this, SIGNAL(sendCommand(int,QSharedPointer<ICmsData>)), m_mainwindow, SIGNAL(sendCommand(int,QSharedPointer<ICmsData>)));
}

DialogIBPSet::~DialogIBPSet()
{
    delete ui;
}

void DialogIBPSet::setLabel(CmsLabelID id, int cnt)
{
    if(cnt > 3)
    {
        qDebug() << " Dialog ibp set label cnt too much";
        return;
    }
    m_LabelID[cnt] = id;
}

void DialogIBPSet::reset()
{
    memset(m_LabelID, 0, sizeof(m_LabelID));
}

void DialogIBPSet::setWaveSpeed(const LabelWave::WaveSpeed &speed)
{
    ui->comboBoxWaveSpeed->setCurrentIndex((int)speed);
}

void DialogIBPSet::on_pushButtonIbp1Zero_clicked()
{
    if(m_pTcpServer == Q_NULLPTR)
    {
        qDebug() << "Tcp Server is null";
        return;
    }
    if(m_iClientId == INVALID_DATA)
    {
        qDebug() << "Client is not be connected!";
        return;
    }
    if(m_LabelID[0] == CMS_LabelNull)
    {
        qDebug() << "ibp1 is null";
        return;
    }

    IbpCommandField d;
    d.command = 0x01;
    auto command = new CmsIbpCommand(CMS_ModuleIbp, m_LabelID[0]);
    command->setCommand(d);
    QSharedPointer<ICmsData> data(command);

    emit sendCommand(m_iClientId, data);
    data.clear();
}

void DialogIBPSet::on_pushButtonIbp2Zero_clicked()
{
    if(m_pTcpServer == Q_NULLPTR)
    {
        qDebug() << "Tcp Server is null";
        return;
    }
    if(m_iClientId == INVALID_DATA)
    {
        qDebug() << "Client is not be connected!";
        return;
    }
    if(m_LabelID[1] == CMS_LabelNull)
    {
        qDebug() << "ibp2 is null";
        return;
    }

    IbpCommandField d;
    d.command = 0x01;
    auto command = new CmsIbpCommand(CMS_ModuleIbp, m_LabelID[1]);
    command->setCommand(d);
    QSharedPointer<ICmsData> data(command);

    emit sendCommand(m_iClientId, data);
    data.clear();
}

void DialogIBPSet::on_pushButtonIbp3Zero_clicked()
{
    if(m_pTcpServer == Q_NULLPTR)
    {
        qDebug() << "Tcp Server is null";
        return;
    }
    if(m_iClientId == INVALID_DATA)
    {
        qDebug() << "Client is not be connected!";
        return;
    }
    if(m_LabelID[2] == CMS_LabelNull)
    {
        qDebug() << "ibp3 is null";
        return;
    }

    IbpCommandField d;
    d.command = 0x01;
    auto command = new CmsIbpCommand(CMS_ModuleIbp, m_LabelID[2]);
    command->setCommand(d);
    QSharedPointer<ICmsData> data(command);

    emit sendCommand(m_iClientId, data);
    data.clear();
}

void DialogIBPSet::on_pushButtonIbp4Zero_clicked()
{
    if(m_pTcpServer == Q_NULLPTR)
    {
        qDebug() << "Tcp Server is null";
        return;
    }
    if(m_iClientId == INVALID_DATA)
    {
        qDebug() << "Client is not be connected!";
        return;
    }
    if(m_LabelID[3] == CMS_LabelNull)
    {
        qDebug() << "ibp4 is null";
        return;
    }

    IbpCommandField d;
    d.command = 0x01;
    auto command = new CmsIbpCommand(CMS_ModuleIbp, m_LabelID[3]);
    command->setCommand(d);
    QSharedPointer<ICmsData> data(command);

    emit sendCommand(m_iClientId, data);
    data.clear();
}

void DialogIBPSet::on_pushButtonIbp1Cal_clicked()
{
    if(m_pTcpServer == Q_NULLPTR)
    {
        qDebug() << "Tcp Server is null";
        return;
    }
    if(m_iClientId == INVALID_DATA)
    {
        qDebug() << "Client is not be connected!";
        return;
    }
    if(m_LabelID[0] == CMS_LabelNull)
    {
        qDebug() << "ibp1 is null";
        return;
    }

    IbpCommandField d;
    d.command = 0x02;
    quint16 press = ui->comboBoxIbp1Cal->currentText().toShort();
    d.param1 = press & 0x00FF;
    d.param2 = (press & 0xFF00) >> 8;
    auto command = new CmsIbpCommand(CMS_ModuleIbp, m_LabelID[0]);
    command->setCommand(d);
    QSharedPointer<ICmsData> data(command);

    emit sendCommand(m_iClientId, data);
    data.clear();
}

void DialogIBPSet::on_pushButtonIbp2Cal_clicked()
{
    if(m_pTcpServer == Q_NULLPTR)
    {
        qDebug() << "Tcp Server is null";
        return;
    }
    if(m_iClientId == INVALID_DATA)
    {
        qDebug() << "Client is not be connected!";
        return;
    }
    if(m_LabelID[1] == CMS_LabelNull)
    {
        qDebug() << "ibp2 is null";
        return;
    }

    IbpCommandField d;
    d.command = 0x02;
    quint16 press = ui->comboBoxIbp2Cal->currentText().toShort();
    d.param1 = press & 0x00FF;
    d.param2 = (press & 0xFF00) >> 8;
    auto command = new CmsIbpCommand(CMS_ModuleIbp, m_LabelID[1]);
    command->setCommand(d);
    QSharedPointer<ICmsData> data(command);

    emit sendCommand(m_iClientId, data);
    data.clear();
}

void DialogIBPSet::on_pushButtonIbp3Cal_clicked()
{
    if(m_pTcpServer == Q_NULLPTR)
    {
        qDebug() << "Tcp Server is null";
        return;
    }
    if(m_iClientId == INVALID_DATA)
    {
        qDebug() << "Client is not be connected!";
        return;
    }
    if(m_LabelID[2] == CMS_LabelNull)
    {
        qDebug() << "ibp3 is null";
        return;
    }

    IbpCommandField d;
    d.command = 0x02;
    quint16 press = ui->comboBoxIbp3Cal->currentText().toShort();
    d.param1 = press & 0x00FF;
    d.param2 = (press & 0xFF00) >> 8;
    auto command = new CmsIbpCommand(CMS_ModuleIbp, m_LabelID[2]);
    command->setCommand(d);
    QSharedPointer<ICmsData> data(command);

    emit sendCommand(m_iClientId, data);
    data.clear();
}

void DialogIBPSet::on_pushButtonIbp4Cal_clicked()
{
    if(m_pTcpServer == Q_NULLPTR)
    {
        qDebug() << "Tcp Server is null";
        return;
    }
    if(m_iClientId == INVALID_DATA)
    {
        qDebug() << "Client is not be connected!";
        return;
    }
    if(m_LabelID[3] == CMS_LabelNull)
    {
        qDebug() << "ibp4 is null";
        return;
    }

    IbpCommandField d;
    d.command = 0x02;
    quint16 press = ui->comboBoxIbp4Cal->currentText().toShort();
    d.param1 = press & 0x00FF;
    d.param2 = (press & 0xFF00) >> 8;
    auto command = new CmsIbpCommand(CMS_ModuleIbp, m_LabelID[3]);
    command->setCommand(d);
    QSharedPointer<ICmsData> data(command);

    emit sendCommand(m_iClientId, data);
    data.clear();
}

void DialogIBPSet::on_buttonBox_accepted()
{
    for(int i = 0; i != m_WaveList.size(); i++)
    {
        m_WaveList[i]->setSpeed((LabelWave::WaveSpeed)ui->comboBoxWaveSpeed->currentIndex());
    }
    emit setMainConfig(LABEL_IBP_START, MainWindow::SetSpeed, ui->comboBoxWaveSpeed->currentIndex());
}
