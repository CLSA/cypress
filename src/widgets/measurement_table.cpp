#include "measurement_table.h"
#include "ui_measurement_table.h"

MeasurementTable::MeasurementTable(QWidget *parent) :
    QWidget(parent), ui(new Ui::MeasurementTable)
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

    // Request measurement
    connect(ui->measureButton, &QPushButton::clicked, this, [=]() {
        emit measure();
    });

    // Measurement manually updated
    connect(ui->measurementTable, &QTableWidget::cellChanged, this, &MeasurementTable::handleChange);

    connect(ui->saveButton, &QPushButton::clicked, this, [=]() {
        emit finish();
    });
}

MeasurementTable::~MeasurementTable()
{
    delete ui;
}


void MeasurementTable::initializeModel(QList<TableColumn> columns)
{
    ui->measurementTable->clear();

    m_columns = columns;

    if (!columns.isEmpty())
    {
        QStringList headerNames;

        ui->measurementTable->setColumnCount(columns.length());

        for (int i = 0; i < columns.length(); i++)
        {
            headerNames << columns[i].displayName;
            ui->measurementTable->setItemDelegateForColumn(i, columns[i].delegate);
        }

        ui->measurementTable->setHorizontalHeaderLabels(headerNames);
    }
}

void MeasurementTable::handleChange(int row, int col)
{
    Measurement& measure = m_test->get(row);
    TableColumn& column = m_columns[col];

    measure.setAttribute(column.key, ui->measurementTable->item(row, col)->data(Qt::EditRole));

    qDebug() << m_test->getMeasurements();
}

void MeasurementTable::updateModel(TestBase<Measurement>* test)
{
    m_test = test;
    if (!m_test)
    {
        return;
    }

    const QVector<Measurement> measurements = m_test->getMeasurements();

    qDebug() << test->getMeasurements();
    qDebug() << "len" << test->getMeasurementCount();
    qDebug() << "valid" << test->isValid();

    ui->measurementTable->setRowCount(0);

    if (!measurements.isEmpty())
    {
        int row = 0;
        int col = 0;

        foreach (auto measurement, measurements)
        {
            ui->measurementTable->insertRow(row);

            foreach (auto column, m_columns)
            {
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setData(Qt::EditRole, measurement.getAttributeValue(column.key));
                ui->measurementTable->setItem(row, col++, item);
            }

            ++row;
            col = 0;
        }
    }
}

void MeasurementTable::handleTestUpdate(TestBase<Measurement>* test)
{
    updateModel(test);
}

void MeasurementTable::enableMeasureButton()
{
    ui->measureButton->setEnabled(true);
}

void MeasurementTable::disableMeasureButton()
{
    ui->measureButton->setEnabled(false);
}

void MeasurementTable::enableFinishButton()
{
    ui->saveButton->setEnabled(true);
}

void MeasurementTable::disableFinishButton()
{
    ui->saveButton->setEnabled(false);
}

void MeasurementTable::setColumnDelegate(int col, QItemDelegate* itemDelegate)
{
    ui->measurementTable->setItemDelegateForColumn(col, itemDelegate);
}


