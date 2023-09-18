#include "measurement_table.h"
#include "ui_measurement_table.h"

MeasurementTable::MeasurementTable(TestBase<Measurement>* test, QWidget *parent) :
    QWidget(parent), m_test(test),
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

    initializeModel();

    connect(ui->measurementTable, &QTableWidget::cellChanged, this, &MeasurementTable::handleChange)
}

MeasurementTable::~MeasurementTable()
{
    delete ui;
}


void MeasurementTable::initializeModel()
{
    auto measurements = m_test->getMeasurements();

    if (!measurements.isEmpty())
    {
        auto firstMeasure = measurements.first();
        auto attributeKeys = firstMeasure.getAttributeKeys();

        int row = 0;
        int col = 0;

        foreach (const auto attributeKey, attributeKeys)
        {
            //QStandardItem* item = new QStandardItem();
            m_model->setHeaderData(0, Qt::Horizontal, attributeKey, Qt::DisplayRole);
            //ui->measurementTable->setItemDelegateForColumn(, col);
        }
    }
}

void MeasurementTable::updateModel()
{
    auto measurements = m_test->getMeasurements();

    if (!measurements.isEmpty())
    {
        auto firstMeasure = measurements.first();
        auto attributeKeys = firstMeasure.getAttributeKeys();

        foreach (auto measurement, measurements)
        {
            foreach (const auto attributeKey, attributeKeys)
            {
                //QStandardItem* item = new QStandardItem();
                m_model->setHeaderData(0, Qt::Horizontal, measurement.getAttribute(attributeKey).value(), Qt::DisplayRole);
            }
        }
    }
}


void MeasurementTable::handleChange(int row, int col)
{
    qDebug() << "cell changed: " << row << col;
}

void MeasurementTable::enableMeasureButton()
{
    ui->measureButton->setEnabled(true);
}

void MeasurementTable::disableMeasureButton()
{
    ui->measureButton->setEnabled(false);
}
