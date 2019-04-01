#ifndef DIALOGRESPSET_H
#define DIALOGRESPSET_H

#include <QDialog>

namespace Ui {
class DialogRESPSet;
}

class DialogRESPSet : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRESPSet(QWidget *parent = 0);
    ~DialogRESPSet();

private:
    Ui::DialogRESPSet *ui;
};

#endif // DIALOGRESPSET_H
