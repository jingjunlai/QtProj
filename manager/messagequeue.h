#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <QObject>

class MessageQueue : public QObject
{
    Q_OBJECT
public:
    explicit MessageQueue(QObject *parent = 0);

signals:

public slots:
};

#endif // MESSAGEQUEUE_H