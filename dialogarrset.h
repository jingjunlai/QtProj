#ifndef DIALOGARRSET_H
#define DIALOGARRSET_H

#include <QDialog>

namespace Ui {
class DialogARRSet;
}

class DialogARRSet : public QDialog
{
    Q_OBJECT

public:
    explicit DialogARRSet(QWidget *parent = 0);
    ~DialogARRSet();

private:
    Ui::DialogARRSet *ui;
};

#endif // DIALOGARRSET_H
