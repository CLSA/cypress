#include "measurement_table.h"
#include "ui_measurement_table.h"

#include "data/Measurement.h"

#include <QMessageBox>
#include <QJsonObject>

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

    ui->addMeasureButton->setVisible(false);

    ui->manualEntryToggle->setFocusPolicy(Qt::NoFocus);

    ui->measurementTable->setStyleSheet(
        "alternate-background-color: #e5e5e5;"
        "background-color: #f5f5f5;"
        "gridline-color: #d3d3d3;"
    );

    // Request automatic measurement
    connect(ui->measureButton, &QPushButton::clicked, this, [=]() {
        if (manualEditsMade || ui->measurementTable->rowCount() > 0)
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(nullptr, "Confirmation", "Are you sure you want to measure again? This will overrite the measurements collected.",
                                          QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                manualEditsMade = false;
            } else {
                return;
            }
        }

        emit measure();
    });

    // Measurement manually updated
    connect(ui->measurementTable, &QTableWidget::cellChanged, this, &MeasurementTable::handleChange);

    connect(ui->submitButton, &QPushButton::clicked, this, [=]() {
        //if (m_test->isValid())
        //{
        emit finish();
        //}
        //else {
         //   QMessageBox::warning(this, "Test Incomplete", "The test is incomplete, please enter all required fields before saving.");
        //}
    });

    connect(ui->manualEntryToggle, &QPushButton::clicked, this, [=]() {
        toggleManualEntry(true);
        emit enterManualEntry();
    });

    connect(ui->addMeasureButton, &QPushButton::clicked, this, &MeasurementTable::addManualMeasurement);

}

MeasurementTable::~MeasurementTable()
{
    delete ui;
}


void MeasurementTable::initializeModel(QList<TableColumn> columns)
{
    ui->measurementTable->clear();

    m_columns = columns;

    setTableColumns();
}

void MeasurementTable::handleChange(int row, int col)
{
    ui->addMeasureButton->setEnabled(ui->measurementTable->rowCount() < m_test->getExpectedMeasurementCount());
    ui->submitButton->setEnabled((ui->measurementTable->rowCount() == m_test->getExpectedMeasurementCount()) && m_test->isValid());
}

void MeasurementTable::updateRowIds()
{
    for (int row = 0; row < ui->measurementTable->rowCount(); row++)
    {
        QWidget* widget = ui->measurementTable->cellWidget(row, ui->measurementTable->columnCount() - 1);
        if (widget)
        {
            QPushButton* button = qobject_cast<QPushButton*>(widget);
            if (button)
            {
                button->setProperty("row_id", row);
            }
        }
    }
}

void MeasurementTable::setTableColumns()
{
    if (!m_columns.isEmpty())
    {
        QStringList headerNames;

        int columnsCount = m_columns.length();

        ui->measurementTable->setColumnCount(columnsCount);

        for (int i = 0; i < m_columns.length(); i++)
        {
            headerNames << m_columns[i].displayName;
            ui->measurementTable->setItemDelegateForColumn(i, m_columns[i].delegate);
        }

        ui->measurementTable->setHorizontalHeaderLabels(headerNames);
    }
}

void MeasurementTable::saveManualChanges()
{
    updateRowIds();

    for (int row = 0; row < ui->measurementTable->rowCount(); row++)
    {
        Measurement& measure = m_test->get(row);

        for (int col = 0; col < ui->measurementTable->columnCount() - 1; col++)
        {
            TableColumn& column = m_columns[col];

            Measurement::Value attribute = measure.getAttribute(column.key);

            bool hasUnits = measure.getAttribute(column.key).hasUnits();

            if (hasUnits)
            {
                QString unit = attribute.units();
                measure.setAttribute(column.key, ui->measurementTable->item(row, col)->data(Qt::EditRole), unit);
            }

            else {
                measure.setAttribute(column.key, ui->measurementTable->item(row, col)->data(Qt::EditRole));
            }
        }
    }

    ui->addMeasureButton->setEnabled(ui->measurementTable->rowCount() < m_test->getExpectedMeasurementCount());
    ui->submitButton->setEnabled((ui->measurementTable->rowCount() == m_test->getExpectedMeasurementCount()) && m_test->isValid());
}

void MeasurementTable::updateModel(TestBase<Measurement>* test)
{
    m_test = test;
    if (!m_test)
    {
        return;
    }

    qDebug() << m_test->toJsonObject();
    const QVector<Measurement> measurements = m_test->getMeasurements();

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

            if (manualEntryMode)
            {
                QPushButton* btn = new QPushButton("Remove", this);

                btn->setProperty("row_id", row);
                btn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

                connect(btn, &QPushButton::clicked, this, &MeasurementTable::removeManualMeasurement);
                ui->measurementTable->setCellWidget(row, col, btn);
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
    ui->submitButton->setEnabled(true);
}

void MeasurementTable::disableFinishButton()
{
    ui->submitButton->setEnabled(false);
}

void MeasurementTable::addManualMeasurement()
{
    emit addMeasurement();
}

void MeasurementTable::removeManualMeasurement()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender()); // retrieve the button clicked
    if (btn)
    {
        int row_id = btn->property("row_id").toInt();
        if (row_id < 0 || row_id >= ui->measurementTable->rowCount()) {
            return;
        }
        m_test->removeMeasurement(row_id);
        updateModel(m_test);
    }
}

void MeasurementTable::setColumnDelegate(int col, QItemDelegate* itemDelegate)
{
    ui->measurementTable->setItemDelegateForColumn(col, itemDelegate);
}

void MeasurementTable::toggleManualEntry(bool saveChanges)
{
    manualEntryMode = !manualEntryMode;

    if (manualEntryMode) {
        ui->measurementTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::AnyKeyPressed);

        ui->manualEntryToggle->setText("Save");
        ui->addMeasureButton->setVisible(true);
        ui->measureButton->setVisible(false);


        int newColumn = ui->measurementTable->columnCount();
        ui->measurementTable->insertColumn(newColumn);

        QTableWidgetItem *header = new QTableWidgetItem("Actions");
        ui->measurementTable->setHorizontalHeaderItem(newColumn, header);

        // Add removal button to each row in actions column
        for (int row = 0; row < ui->measurementTable->rowCount(); row++)
        {
            QPushButton* btn = new QPushButton("Remove", this);

            btn->setProperty("row_id", row);
            btn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

            connect(btn, &QPushButton::clicked, this, &MeasurementTable::removeManualMeasurement);
            ui->measurementTable->setCellWidget(row, newColumn, btn);
        }
    }
    else {
        if (saveChanges)
        {
            saveManualChanges();
        }

        ui->manualEntryToggle->setText("Manual Entry");
        ui->addMeasureButton->setVisible(false);
        ui->measureButton->setVisible(true);

        ui->measurementTable->removeColumn(ui->measurementTable->columnCount() - 1);
        ui->measurementTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        ui->measurementTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}


