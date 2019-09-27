#ifndef DIALOGECGSET_H
#define DIALOGECGSET_H

#include "module/moduledata.h"
#include "net/cmsecgconfigdata.h"
#include "idialogset.h"
#include "labelwave.h"
#include <QButtonGroup>

namespace Ui {
class DialogECGSet;
}

class MyTcpServer;
class DialogECGSet : public IDialogSet
{
    Q_OBJECT

public:
    explicit DialogECGSet(QWidget *parent = 0);

    ~DialogECGSet();

    void setWaveSpeed(const LabelWave::WaveSpeed &speed);

signals:


private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogECGSet *ui;

    QButtonGroup        *m_pButtonGroupGain = Q_NULLPTR;
    QButtonGroup        *m_pButtonGroupMode = Q_NULLPTR;
};

#endif // DIALOGECGSET_H
