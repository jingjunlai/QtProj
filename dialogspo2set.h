#ifndef DIALOGSPO2SET_H
#define DIALOGSPO2SET_H

#include "idialogset.h"
#include "labelwave.h"

namespace Ui {
class DialogSPO2Set;
}

class DialogSPO2Set : public IDialogSet
{
    Q_OBJECT

public:

    typedef enum
    {
        Spo2 = 0,
        Resp,
    } WaveID;

    explicit DialogSPO2Set(QWidget *parent = 0);

    ~DialogSPO2Set();

    void setSpo2WaveSpeed(const LabelWave::WaveSpeed &speed);

    void setRespWaveSpeed(const LabelWave::WaveSpeed &speed);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogSPO2Set *ui;
};

#endif // DIALOGSPO2SET_H
