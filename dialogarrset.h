#ifndef DIALOGARRSET_H
#define DIALOGARRSET_H

#include "idialogset.h"
#include "labelwave.h"

namespace Ui {
class DialogARRSet;
}

class DialogARRSet : public IDialogSet
{
    Q_OBJECT

public:
    explicit DialogARRSet(QWidget *parent = 0);

    ~DialogARRSet();

    void setWaveSpeed(const LabelWave::WaveSpeed &speed);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogARRSet *ui;
};

#endif // DIALOGARRSET_H
