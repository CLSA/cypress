#include "cimt_vivid_iq_dialog.h"
#include "ui_cimt_vivid_iq_dialog.h"

CimtVividIQDialog::CimtVividIQDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CimtVividIQDialog)
{
    ui->setupUi(this);
}

CimtVividIQDialog::~CimtVividIQDialog()
{
    delete ui;
}
