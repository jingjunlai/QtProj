#ifndef IMODULE_H
#define IMODULE_H

#include <QObject>
#include "moduleproperty.h"
#include "moduledata.h"


class IModule : public QObject
{
    Q_OBJECT
public:
    explicit IModule(ModuleProperty property, QObject *parent = nullptr);

    virtual ~IModule();

    ModuleProperty moduleProperty() const;

    bool Online() const;

    virtual bool setConfig(int id, const QVariant &value) = 0;

    virtual bool getConfig(int id, QVariant &value) = 0;

    virtual void unpackModuleData(const QByteArray &packet) = 0;

    virtual void setData(const QVariant &value) = 0;

    virtual void getData(QVariant &value) = 0;

    void setEnable();

    void setOnline();

signals:

public slots:

protected:

    ModuleProperty      m_ModuleProperty;
    bool                m_bOnline = false;
};

#endif // IMODULE_H
