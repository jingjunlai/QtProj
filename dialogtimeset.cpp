#include "dialogtimeset.h"
#include "ui_dialogtimeset.h"

DialogTimeSet::DialogTimeSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTimeSet)
{
    ui->setupUi(this);
}

DialogTimeSet::~DialogTimeSet()
{
    delete ui;
}
