#include "ecgdialog.h"
#include "ui_ecgdialog.h"

EcgDialog::EcgDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EcgDialog)
{
    ui->setupUi(this);
}

EcgDialog::~EcgDialog()
{
    delete ui;
}
