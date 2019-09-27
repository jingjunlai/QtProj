#include "portmanager.h"
#include "serialport.h"
#include "nbmprotocolparser.h"
#include <QMutex>
#include <QThread>
#include <QDebug>

Q_GLOBAL_STATIC(QMutex, lock)

PortManager *PortManager::m_pInstance = Q_NULLPTR;

PortManager::~PortManager()
{

}

PortManager *PortManager::instance()
{
    if(m_pInstance == Q_NULLPTR)
    {
        QMutexLocker locker(lock());
        if(m_pInstance == Q_NULLPTR)
        {
            m_pInstance = new PortManager();
        }
    }

    return m_pInstance;
}

PortManager::PortManager(QObject *parent)
    : IManager(parent)
{

}

void PortManager::doOpen()
{
    QMutexLocker locker(lock());

    if(m_SerialSetting.port == "")
        return;

    m_SerialPorts = new SerialPort(m_SerialSetting);
    QThread *th = new QThread();
    m_SerialPorts->moveToThread(th);
    connect(th, SIGNAL(started()), m_SerialPorts, SLOT(start()));
    connect(th, SIGNAL(finished()), m_SerialPorts, SLOT(deleteLater()));
    connect(th, SIGNAL(finished()), th, SLOT(deleteLater()));
    m_SerialPorts->setParser(new NbmProtocolParser());

    th->start();
    th->setObjectName("nbm_serialport");
    th->setPriority(QThread::HighPriority);
}

void PortManager::doClose()
{
    if(m_SerialPorts == Q_NULLPTR)
        return;

    if(m_SerialPorts->thread() == Q_NULLPTR)
        delete m_SerialPorts;
    else
        m_SerialPorts->thread()->quit();
}

void PortManager::setSerialPortEnable(bool en)
{
    QMutexLocker locker(lock());
    Q_UNUSED(locker)

    if(m_SerialPorts == Q_NULLPTR)
        return;

    if (en)
    {
        if (!m_SerialPorts->isOpen())
            m_SerialPorts->open(QIODevice::ReadWrite);
    }
    else
    {
        if (m_SerialPorts->isOpen())
            m_SerialPorts->close();
    }
}

void PortManager::init(PortManager *me, const QEvent *event)
{
    switch ((int)event->type())
    {
        case EV_Init:
            me->doOpen();
            me->transform(MANAGER_TRANS(PortManager::worker));
            break;
        default:
            break;
    }
}

void PortManager::worker(PortManager *me, const QEvent *event)
{
    switch ((int)event->type())
    {
        case EV_Quit:
            me->doClose();
            me->transform(MANAGER_TRANS(IManager::quit));
            break;
        case EV_TriggerSystemEvent:
            {
//                VariantEv *p = (VariantEv*)event;
//                auto ev = p->var.value<Gps::SystemEvent>();
//                me->triggerSystemEvent(ev);
            } break;
        case EV_ResponseTriggerSystemEvent:
            {
//                VariantEv *p = (VariantEv*)event;
//                auto ev = p->var.value<Gps::ResponseToDemo>();
//                if (ev.result)
//                {
//                    me->toDemo();
//                }
            } break;
        default:
            break;
    }
}

void PortManager::start()
{
    subscribe(EV_Init);
    subscribe(EV_Quit);
    subscribe(EV_TriggerSystemEvent);
    subscribe(EV_ResponseTriggerSystemEvent);
    transform(MANAGER_TRANS(PortManager::init));
}

void PortManager::write(const QByteArray &packet)
{
    QMutexLocker locker(lock());

    if(m_SerialPorts == Q_NULLPTR)
        return;

    m_SerialPorts->write(packet);
}

void PortManager::setSerialSetting(const SerialSetting &setting)
{
    m_SerialSetting = setting;
}


