#include "GripStrengthDialog.h"
#include "ui_gripstrengthdialog.h"

#include <QDebug>
#include <QMessageBox>

GripStrengthDialog::GripStrengthDialog(QWidget *parent):
    DialogBase(parent),
    ui(new Ui::GripStrengthDialog)
{
    ui->setupUi(this);
}

GripStrengthDialog::~GripStrengthDialog()
{
    delete ui;
}
