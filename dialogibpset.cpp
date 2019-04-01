#include "dialogibpset.h"
#include "ui_dialogibpset.h"

DialogIBPSet::DialogIBPSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogIBPSet)
{
    ui->setupUi(this);
}

DialogIBPSet::~DialogIBPSet()
{
    delete ui;
}
