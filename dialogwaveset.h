#ifndef DIALOGWAVESET_H
#define DIALOGWAVESET_H

//#include <QDialog>
#include "idialogset.h"
#include <QButtonGroup>

namespace Ui {
class DialogWaveSet;
}

class DialogWaveSet : public IDialogSet//QDialog
{
    Q_OBJECT

public:
    typedef enum
    {
        Default = 0,
        Ecg12Lead,
        Custom
    } LeadMode;

    explicit DialogWaveSet(QWidget *parent = 0);

    ~DialogWaveSet();

    void setWaveLead(DialogWaveSet::LeadMode mode);

signals:

    void setWaveLead(DialogWaveSet::LeadMode mode, quint32 param);

private slots:
    void on_buttonBox_accepted();

    void on_comboBoxLeadSet_currentIndexChanged(int index);

private:
    Ui::DialogWaveSet *ui;

    QButtonGroup        *m_pButtonGroupLead;
};

#endif // DIALOGWAVESET_H
