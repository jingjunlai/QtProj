#include "dialogarrset.h"
#include "ui_dialogarrset.h"

DialogARRSet::DialogARRSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogARRSet)
{
    ui->setupUi(this);
}

DialogARRSet::~DialogARRSet()
{
    delete ui;
}
