#include "messagequeue.h"

#include <QCoreApplication>

MessageQueue::MessageQueue(QObject *parent)
    : QObject(parent)
{

}

MessageQueue::~MessageQueue()
{

}

void MessageQueue::publish(int id, const QEvent* event)
{
    QMutexLocker locker(&mutex);

    CustomEvent* e = (CustomEvent*)event;
    if (e == nullptr)
        return;

    if (subscriber.contains(id))
    {
        QList<QObject*> value = subscriber.value(id);
        foreach (QObject* v, value)
        {
            QCoreApplication::postEvent(v, e->clone());
        }
    }

    delete event;
}

void MessageQueue::publish(QObject *obj, int id, const QEvent *event)
{
    QMutexLocker locker(&mutex);

    CustomEvent* e = (CustomEvent*)event;
    if (e == nullptr)
        return;

    if (subscriber.contains(id))
    {
        QList<QObject*> value = subscriber.value(id);
        foreach (QObject* v, value)
        {
            if (v == obj)
                QCoreApplication::postEvent(v, e->clone());
        }
    }

    delete event;
}

void MessageQueue::subscribe(int id, QObject* who)
{
    QMutexLocker locker(&mutex);

    if (who == nullptr)
        return;

    if (subscriber.contains(id))
    {
        QList<QObject*> value = subscriber.value(id);
        if (value.contains(who))
            return;

        value.push_back(who);
        subscriber.insert(id, value);
        return;
    }

    QList<QObject*> value;
    value.push_back(who);
    subscriber.insert(id, value);
}
