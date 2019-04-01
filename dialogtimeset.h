#ifndef DIALOGTIMESET_H
#define DIALOGTIMESET_H

#include <QDialog>

namespace Ui {
class DialogTimeSet;
}

class DialogTimeSet : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTimeSet(QWidget *parent = 0);
    ~DialogTimeSet();

private:
    Ui::DialogTimeSet *ui;
};

#endif // DIALOGTIMESET_H
