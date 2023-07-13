#include "measurement_table.h"
#include "ui_measurement_table.h"

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


void MeasurementTable::enableMeasureButton()
{
    ui->measureButton->setEnabled(true);
}

void MeasurementTable::disableMeasureButton()
{
    ui->measureButton->setEnabled(false);
}
