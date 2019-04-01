#ifndef DIALOGSPO2SET_H
#define DIALOGSPO2SET_H

#include <QDialog>

namespace Ui {
class DialogSPO2Set;
}

class DialogSPO2Set : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSPO2Set(QWidget *parent = 0);
    ~DialogSPO2Set();

private:
    Ui::DialogSPO2Set *ui;
};

#endif // DIALOGSPO2SET_H
