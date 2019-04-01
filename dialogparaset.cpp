#include "dialogparaset.h"
#include "ui_dialogparaset.h"

DialogParaSet::DialogParaSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogParaSet)
{
    ui->setupUi(this);
}

DialogParaSet::~DialogParaSet()
{
    delete ui;
}
