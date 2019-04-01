#include "dialogrespset.h"
#include "ui_dialogrespset.h"

DialogRESPSet::DialogRESPSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRESPSet)
{
    ui->setupUi(this);
}

DialogRESPSet::~DialogRESPSet()
{
    delete ui;
}
