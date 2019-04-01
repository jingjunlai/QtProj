#include "dialogecgset.h"
#include "ui_dialogecgset.h"

DialogECGSet::DialogECGSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogECGSet)
{
    ui->setupUi(this);
}

DialogECGSet::~DialogECGSet()
{
    delete ui;
}
