#ifndef DIALOGIBPSET_H
#define DIALOGIBPSET_H

#include "idialogset.h"
#include "labelwave.h"

namespace Ui {
class DialogIBPSet;
}

class DialogIBPSet : public IDialogSet
{
    Q_OBJECT

public:
    explicit DialogIBPSet(QWidget *parent = 0);

    ~DialogIBPSet();

    void setLabel(CmsLabelID id, int cnt);

    void reset();

    void setWaveSpeed(const LabelWave::WaveSpeed &speed);

signals:

    void sendCommand(int id, const QSharedPointer<ICmsData> data);

private slots:

    void on_pushButtonIbp1Zero_clicked();

    void on_pushButtonIbp2Zero_clicked();

    void on_pushButtonIbp3Zero_clicked();

    void on_pushButtonIbp4Zero_clicked();

    void on_pushButtonIbp1Cal_clicked();

    void on_pushButtonIbp2Cal_clicked();

    void on_pushButtonIbp3Cal_clicked();

    void on_pushButtonIbp4Cal_clicked();

    void on_buttonBox_accepted();

private:
    Ui::DialogIBPSet *ui;

    CmsLabelID          m_LabelID[4] = {CMS_LabelNull};
};

#endif // DIALOGIBPSET_H
