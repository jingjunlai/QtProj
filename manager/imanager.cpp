#include "imanager.h"

IManager::IManager(QObject *parent)
    : QObject(parent)
    , callFunc(nullptr)
    , messageQ(nullptr)
{

}

IManager::~IManager()
{

}

void IManager::setMessageQueue(MessageQueue *q)
{
    if (messageQ == nullptr)
        messageQ = q;
}

MessageQueue *IManager::getMessageQueue() const
{
    return messageQ;
}

void IManager::subscribe(int id)
{
    if (messageQ != nullptr)
        messageQ->subscribe(id, this);
}

void IManager::publish(int id, const QEvent *event)
{
    if (messageQ != nullptr)
        messageQ->publish(id, event);
}

void IManager::publish(QObject *obj, int id, const QEvent *event)
{
    if (messageQ != nullptr)
        messageQ->publish(obj, id, event);
}

void IManager::transform(void (*func)(void *, const QEvent *))
{
    callFunc = func;
}

void IManager::quit(IManager *me, const QEvent *event)
{
    Q_UNUSED(me)
    Q_UNUSED(event)
    /* null */
}

void IManager::customEvent(QEvent *event)
{
    if (callFunc != nullptr)
        callFunc(this, event);
//    QObject::customEvent(event);
}
