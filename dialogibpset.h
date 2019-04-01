#ifndef DIALOGIBPSET_H
#define DIALOGIBPSET_H

#include <QDialog>

namespace Ui {
class DialogIBPSet;
}

class DialogIBPSet : public QDialog
{
    Q_OBJECT

public:
    explicit DialogIBPSet(QWidget *parent = 0);
    ~DialogIBPSet();

private:
    Ui::DialogIBPSet *ui;
};

#endif // DIALOGIBPSET_H
