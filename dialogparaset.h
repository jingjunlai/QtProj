#ifndef DIALOGPARASET_H
#define DIALOGPARASET_H

#include <QDialog>

namespace Ui {
class DialogParaSet;
}

class DialogParaSet : public QDialog
{
    Q_OBJECT

public:
    explicit DialogParaSet(QWidget *parent = 0);
    ~DialogParaSet();

private:
    Ui::DialogParaSet *ui;
};

#endif // DIALOGPARASET_H
