#ifndef IMANAGER_H
#define IMANAGER_H

#include <QObject>
#include "eventid.h"
#include "messagequeue.h"

class IManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(IManager)

public:
    typedef void (*CallFunc)(void *me, QEvent const *event);

    explicit IManager(QObject *parent = nullptr);

    virtual ~IManager();

    virtual void start() = 0;

    void setMessageQueue(MessageQueue *q);

    MessageQueue *getMessageQueue() const;

    void subscribe(int id);

    void publish(int id, const QEvent* event);

    void publish(QObject *obj, int id, const QEvent* event);

protected:
    void transform(void (*func)(void *me, const QEvent *event));

    static void quit(IManager *me, QEvent const *event);

    virtual void customEvent(QEvent *event);

private:
    CallFunc                callFunc;

    MessageQueue        *messageQ;
};

#define MANAGER_TRANS(obj) (reinterpret_cast<IManager::CallFunc>(obj))

#endif // IMANAGER_H
