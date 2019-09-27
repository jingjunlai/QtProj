#ifndef IMANAGER_H
#define IMANAGER_H

#include <QObject>

class IManager : public QObject
{
    Q_OBJECT
public:
    explicit IManager(QObject *parent = 0);

signals:

public slots:
};

#endif // IMANAGER_H