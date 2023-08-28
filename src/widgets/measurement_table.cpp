#include "measurement_table.h"
#include "ui_measurement_table.h"

MeasurementTable::MeasurementTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeasurementTable)
{
    ui->setupUi(this);

    ui->measurementTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->measurementTable->horizontalHeader()->setSectionsClickable(false);

    ui->measurementTable->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->measurementTable->setShowGrid(true);
    ui->measurementTable->setGridStyle(Qt::SolidLine);
    ui->measurementTable->setAlternatingRowColors(true);

    ui->measurementTable->setStyleSheet(
        "alternate-background-color: #e5e5e5;"
        "background-color: #f5f5f5;"
        "gridline-color: #d3d3d3;"
    );
}

MeasurementTable::~MeasurementTable()
{
    delete ui;
}


QTableWidget* MeasurementTable::getTable()
{
    return ui->measurementTable;
}


void MeasurementTable::enableMeasureButton()
{
    ui->measureButton->setEnabled(true);
}

void MeasurementTable::disableMeasureButton()
{
    ui->measureButton->setEnabled(false);
}
