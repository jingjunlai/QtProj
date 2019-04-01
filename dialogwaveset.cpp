#include "dialogwaveset.h"
#include "ui_dialogwaveset.h"

DialogWaveSet::DialogWaveSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWaveSet)
{
    ui->setupUi(this);
}

DialogWaveSet::~DialogWaveSet()
{
    delete ui;
}
