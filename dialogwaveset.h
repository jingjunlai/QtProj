#ifndef DIALOGWAVESET_H
#define DIALOGWAVESET_H

#include <QDialog>

namespace Ui {
class DialogWaveSet;
}

class DialogWaveSet : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWaveSet(QWidget *parent = 0);
    ~DialogWaveSet();

private:
    Ui::DialogWaveSet *ui;
};

#endif // DIALOGWAVESET_H
