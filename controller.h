#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    virtual ~Controller();
    void searchPort();
    bool openPort();
    void closePort();

signals:

public slots:
};

#endif // CONTROLLER_H
