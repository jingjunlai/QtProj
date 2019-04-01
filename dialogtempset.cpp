#include "dialogtempset.h"
#include "ui_dialogtempset.h"

DialogTEMPSet::DialogTEMPSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTEMPSet)
{
    ui->setupUi(this);
}

DialogTEMPSet::~DialogTEMPSet()
{
    delete ui;
}
