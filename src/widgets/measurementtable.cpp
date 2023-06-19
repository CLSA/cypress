#include "measurementtable.h"
#include "ui_measurementtable.h"

MeasurementTable::MeasurementTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeasurementTable)
{
    ui->setupUi(this);
}

MeasurementTable::~MeasurementTable()
{
    delete ui;
}
