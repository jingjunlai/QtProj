#include "model.h"
#include "module/imodule.h"
#include "module/moduleecg.h"
#include "module/moduleproperty.h"


Model::Model(QObject *parent) : QObject(parent)
{

}

Model::~Model()
{
    qDebug()<<"~Model ID = "<<QThread::currentThreadId();
    qDeleteAll(m_ModuleList);
}

void Model::setPort(MySerialPort * const pThreadPort)
{
    if(pThreadPort != NULL)
    {
        m_pSerialport = pThreadPort;
        connect(m_pSerialport, SIGNAL(sendDataToModel(QByteArray)), this, SLOT(handleData(QByteArray)), Qt::QueuedConnection);
//        m_pTimer = new QTimer;
//        connect(m_pTimer, SIGNAL(timeout()), this, SLOT(receiveData()), Qt::QueuedConnection);
//        m_pTimer->start(10);
    }
}

IModule *Model::getModule(LabelID label)
{
    foreach(auto module, m_ModuleList)
    {
        ModuleProperty tmp = module->moduleProperty();
        if(tmp.labelId == label)
        {
            module->setEnable();
            return module;
        }
    }
    return nullptr;
}

bool Model::setModuleConfig(LabelID label, int id, const QVariant &value)
{
    if (IModule *m = getModule(label))
        return m->setConfig(id, value);
    else
        return false;
}

void Model::setModuleConfig(LabelID label, const QVariant *value)
{
    IModule *m = getModule(label);
    if(m)
    {
        m->setConfig(label, *value);
    }
    else
    {
        delete value;
        qDebug() << " get Module failed";
        return;
    }

    delete value;
}

void Model::getModuleConfig(LabelID label, int id)
{
    QVariant *pvalue = new QVariant;
    if (IModule *m = getModule(label))
        m->getConfig(id, *pvalue);
    else
        qDebug() << "get Module failed!";

    emit sendConfigToMain(label, id, pvalue);
}

void Model::setModuleData(LabelID label, const QVariant *value)
{
    if(label == LABEL_ARR)
    {
        IModule *m = getModule(LABEL_ECG);
        if(m)
        {
            ModuleEcg *mEcg = dynamic_cast<ModuleEcg *>(m);
            mEcg->setArrData(value);
        }
    }
    else
    {
        IModule *m = getModule(label);
        if (m)
        {

            m->setData(*value);
        }
        else
        {
            delete value;
            qDebug() << " get Module failed";
            return;
        }
    }

    delete value;
}

void Model::getModuleData(LabelID label)
{
    QVariant *pvalue = new QVariant;
    if (IModule *m = getModule(label))
        m->getData(*pvalue);
    else
        qDebug() << "get Module failed!";

    //emit sendModuleDataToMain(label, pvalue);
}

void Model::sendModuleData()
{
    for(int i = 0; i != m_ModuleList.size(); i++)
    {
        if((m_ModuleList.at(i)->moduleProperty().enable == true) && (m_ModuleList.at(i)->Online() == false))
        {
            QVariant v;
            m_ModuleList.at(i)->getData(v);

            emit sendModuleDataToMain(m_ModuleList.at(i)->moduleProperty().labelId, v);

            m_ModuleList.at(i)->setOnline();
        }
    }
}



void Model::addPortCnt(unsigned int uiSize)
{
    emit sendPortDataSizeToMain(uiSize);
}




