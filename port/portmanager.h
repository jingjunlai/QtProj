#ifndef PORTMANAGER_H
#define PORTMANAGER_H

#include <QObject>

class PortManager : public QObject
{
    Q_OBJECT
public:
    explicit PortManager(QObject *parent = 0);

signals:

public slots:
};

#endif // PORTMANAGER_H