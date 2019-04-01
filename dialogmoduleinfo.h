#ifndef DIALOGMODULEINFO_H
#define DIALOGMODULEINFO_H

#include <QDialog>

namespace Ui {
class DialogModuleInfo;
}

class DialogModuleInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModuleInfo(QWidget *parent = 0);
    ~DialogModuleInfo();

private:
    Ui::DialogModuleInfo *ui;
};

#endif // DIALOGMODULEINFO_H
