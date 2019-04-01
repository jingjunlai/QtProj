#ifndef MODEL_H
#define MODEL_H

#include <QObject>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = 0);
    virtual ~Model();

    virtual void reciveData(const QString &str) = 0;

signals:

public slots:
    virtual void handleData(QByteArray Data) = 0;
};

#endif // MODEL_H
