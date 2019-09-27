#include "idialogset.h"
#include "net/mytcpserver.h"
#include "labelwave.h"
#include "mainwindow.h"

IDialogSet::IDialogSet(QWidget *parent)
    : QDialog(parent)
{
    m_mainwindow = (MainWindow *)parent;
    connect(this, SIGNAL(setMainConfig(LabelID,int,int)), m_mainwindow, SLOT(changeMainConfig(LabelID,int,int)));
}

IDialogSet::~IDialogSet()
{

}

void IDialogSet::setClientId(int id)
{
    m_iClientId = id;
}

void IDialogSet::setServerPointer(MyTcpServer *&p)
{
    if(p == Q_NULLPTR)
        return;

    m_pTcpServer = p;
    qRegisterMetaType<QSharedPointer<ICmsData>>("QSharedPointer<ICmsData>");
    connect(this, SIGNAL(sendConfigData(int, QSharedPointer<ICmsData>)), m_pTcpServer, SLOT(transmitConfigData(int, const QSharedPointer<ICmsData>)), Qt::QueuedConnection);
}

void IDialogSet::setWavePointer(LabelWave *p)
{
    if(p == Q_NULLPTR)
        return;
    m_WaveList.append(p);
}

