#include "dialogspo2set.h"
#include "ui_dialogspo2set.h"

DialogSPO2Set::DialogSPO2Set(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSPO2Set)
{
    ui->setupUi(this);
}

DialogSPO2Set::~DialogSPO2Set()
{
    delete ui;
}
