#ifndef PORTMANAGER_H
#define PORTMANAGER_H

#include <QObject>
#include "manager/imanager.h"
#include "serialport.h"
#include "serialsetting.h"
#include <QMap>

class SerialPort;
class PortManager : public IManager
{
    Q_OBJECT

public:

    virtual ~PortManager();

    static PortManager *instance();

    virtual void start();

    void write(const QByteArray &packet);

    void setSerialSetting(const SerialSetting &setting);

protected:

    explicit PortManager(QObject *parent = 0);

    void doOpen();

    void doClose();

    void setSerialPortEnable(bool en);

    static void init(PortManager *me, QEvent const *event);

    static void worker(PortManager *me, QEvent const *event);

private:

    static PortManager  *m_pInstance;
    SerialPort          *m_SerialPorts = Q_NULLPTR;
    SerialSetting       m_SerialSetting;

};

#define portManager PortManager::instance()

#endif // PORTMANAGER_H
