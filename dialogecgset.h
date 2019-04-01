#ifndef DIALOGECGSET_H
#define DIALOGECGSET_H

#include <QDialog>

namespace Ui {
class DialogECGSet;
}

class DialogECGSet : public QDialog
{
    Q_OBJECT

public:
    explicit DialogECGSet(QWidget *parent = 0);
    ~DialogECGSet();

private:
    Ui::DialogECGSet *ui;
};

#endif // DIALOGECGSET_H
