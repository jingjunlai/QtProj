#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "mythreadserial.h"
#include "module/moduleid.h"
#include "module/moduledata.h"

class IModule;
class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    virtual ~Model();

    void setPort(MySerialPort * const pThreadPort);

    IModule *getModule(LabelID label);

signals:

    void sendPortDataSizeToMain(unsigned int uiSize);

    void sendConfigToMain(LabelID label, int id, const QVariant *value);

    void sendModuleDataToMain(LabelID label, const QVariant &value);

    void enableMainWork(int id);

    void sendUserType(int type);

public slots:

    virtual void handleData(const QByteArray &data) = 0;

    virtual void handleSocketData(const QByteArray &data) = 0;

    virtual void receiveData() = 0;

    virtual void doWork() = 0;

    bool setModuleConfig(LabelID label, int id, const QVariant &value);

    void setModuleConfig(LabelID label, const QVariant *value);

    void getModuleConfig(LabelID label, int id);

    void setModuleData(LabelID label, const QVariant *value);

    void getModuleData(LabelID label);

    void sendModuleData();

protected:

    void addPortCnt(unsigned int uiSize);

    QRingBuffer             m_Ringbuffer;
    QTimer                  *m_pTimer;
    QList<IModule *>        m_ModuleList;
    MySerialPort            *m_pSerialport;
};

#endif // MODEL_H
