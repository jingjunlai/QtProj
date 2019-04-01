#ifndef DIALOGTEMPSET_H
#define DIALOGTEMPSET_H

#include <QDialog>

namespace Ui {
class DialogTEMPSet;
}

class DialogTEMPSet : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTEMPSet(QWidget *parent = 0);
    ~DialogTEMPSet();

private:
    Ui::DialogTEMPSet *ui;
};

#endif // DIALOGTEMPSET_H
