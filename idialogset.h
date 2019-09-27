#ifndef IDIALOGSET_H
#define IDIALOGSET_H

#include "module/moduledata.h"
#include "net/cmsconfigdata.h"
#include <QDialog>

class LabelWave;
class MyTcpServer;
class MainWindow;
class IDialogSet : public QDialog
{
    Q_OBJECT

public:
    IDialogSet(QWidget *parent = 0);

    virtual ~IDialogSet();

    void setClientId(int id);

    void setServerPointer(MyTcpServer * &p);

    void setWavePointer(LabelWave *p);

signals:

    void sendConfigData(int id, const QSharedPointer<ICmsData> data);

    void setMainConfig(LabelID id, int param1, int param2);

protected:

    int                 m_iClientId = INVALID_DATA;
    MyTcpServer         *m_pTcpServer = Q_NULLPTR;
    QList<LabelWave*>   m_WaveList;
    MainWindow          *m_mainwindow;


};

#endif // IDIALOGSET_H
