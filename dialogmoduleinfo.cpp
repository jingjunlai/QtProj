#include "dialogmoduleinfo.h"
#include "ui_dialogmoduleinfo.h"

DialogModuleInfo::DialogModuleInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogModuleInfo)
{
    ui->setupUi(this);
}

DialogModuleInfo::~DialogModuleInfo()
{
    delete ui;
}
