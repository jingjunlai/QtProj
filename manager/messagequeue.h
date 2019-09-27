#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include "event.h"
#include <QObject>
#include <QMap>
#include <QMutex>

class MessageQueue : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(MessageQueue)

public:
    explicit MessageQueue(QObject* parent = nullptr);

    virtual ~MessageQueue();

    void publish(int id, const QEvent* event);

    void publish(QObject *obj, int id, const QEvent* event);

    void subscribe(int id, QObject* who);

private:
    QMap<int, QList<QObject*>> subscriber;
    mutable QMutex mutex;
};

#endif // MESSAGEQUEUE_H
