#ifndef DIALOGRESPSET_H
#define DIALOGRESPSET_H

#include "idialogset.h"
#include "labelwave.h"
#include <QButtonGroup>

namespace Ui {
class DialogRESPSet;
}

class DialogRespSet : public IDialogSet
{
    Q_OBJECT

public:
    explicit DialogRespSet(QWidget *parent = 0);

    ~DialogRespSet();

    void setWaveSpeed(const LabelWave::WaveSpeed &speed);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogRESPSet *ui;

    QButtonGroup        *m_pButtonGroupGain;
    QButtonGroup        *m_pButtonGroupLead;
};

#endif // DIALOGRESPSET_H
