#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QObject>

class MySocket : public QObject
{
    Q_OBJECT
public:
    explicit MySocket(QObject *parent = 0);

signals:

public slots:
};

#endif // MYSOCKET_H
