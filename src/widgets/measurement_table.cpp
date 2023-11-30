#include "measurement_table.h"
#include "ui_measurement_table.h"

#include "data/measurement.h"

#include <QMessageBox>
#include <QJsonObject>

MeasurementTable::MeasurementTable(QWidget *parent) :
    QWidget(parent), ui(new Ui::MeasurementTable)
{
    ui->setupUi(this);

    // Disable measure and complete buttons by default
    ui->measureButton->setEnabled(false);
    ui->submitButton->setEnabled(false);

    // Table setup
    ui->measurementTable->setShowGrid(true);
    ui->measurementTable->setGridStyle(Qt::SolidLine);
    ui->measurementTable->setAlternatingRowColors(true);
    ui->measurementTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->measurementTable->horizontalHeader()->setSectionsClickable(false);
    ui->measurementTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->measurementTable->setStyleSheet(
        "alternate-background-color: #e5e5e5;"
        "background-color: #f5f5f5;"
        "gridline-color: #d3d3d3;"
    );

    // Manual entry setup
    ui->manualEntryToggle->setFocusPolicy(Qt::NoFocus);
    ui->addMeasureButton->setVisible(false);

    // Request automatic measurement
    connect(ui->measureButton, &QPushButton::clicked, this, [=]() {
        if (manualEditsMade)
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
        if (m_test->isValid())
        {
            emit finish();
        }
        else {
            QMessageBox::warning(this, "Test Incomplete", "The test is incomplete or invalid.");
        }
    });

    // Manual entry
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
    Q_UNUSED(row)
    Q_UNUSED(col)

    //ui->addMeasureButton->setEnabled(ui->measurementTable->rowCount() < m_test->getExpectedMeasurementCount());
    //ui->submitButton->setEnabled((ui->measurementTable->rowCount() == m_test->getExpectedMeasurementCount()) && m_test->isValid());
}

void MeasurementTable::updateRowIds()
{
    qDebug() << "update row ids";

    for (int row = 0; row < ui->measurementTable->rowCount(); row++) {
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

    qDebug() << "update rows: " << ui->measurementTable->rowCount();

    for (int row = 0; row < ui->measurementTable->rowCount(); row++) {
        qDebug() << "get measure";

        Measurement &measure = m_test->get(row);

        for (int col = 0; col < ui->measurementTable->columnCount() - 1; col++) {
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

    qDebug() << "before set buttons";
    ui->addMeasureButton->setEnabled(ui->measurementTable->rowCount()
                                     < m_test->getExpectedMeasurementCount());
    qDebug() << "2";
    ui->submitButton->setEnabled((ui->measurementTable->rowCount() == m_test->getExpectedMeasurementCount()) && m_test->isValid());
    qDebug() << "set buttons";
}

void MeasurementTable::updateModel(TestBase* test)
{
    m_test = test;
    if (!m_test)
    {
        return;
    }

    qDebug() << m_test->toJsonObject();

    const QVector<QSharedPointer<Measurement>> measurements = m_test->getMeasurements();

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
                item->setData(Qt::EditRole, measurement->getAttributeValue(column.key));
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

void MeasurementTable::handleTestUpdate(TestBase* test)
{
    updateModel(test);
}

void MeasurementTable::handleDicomFiles(QList<DicomFile> files)
{
    int row = 0;

    ui->measurementTable->clear();
    ui->measurementTable->setRowCount(0);
    ui->measurementTable->setColumnCount(4);

    QStringList headerNames = {"StudyID", "Name", "Type", "Size"};

    ui->measurementTable->setHorizontalHeaderLabels(headerNames);

    foreach (auto file, files) {
        ui->measurementTable->insertRow(row);

        QTableWidgetItem *study = new QTableWidgetItem(file.studyId);
        ui->measurementTable->setItem(row, 0, study);

        QTableWidgetItem *name = new QTableWidgetItem(file.name);
        ui->measurementTable->setItem(row, 1, name);

        QTableWidgetItem *type = new QTableWidgetItem(file.bodyPartExamined);
        ui->measurementTable->setItem(row, 2, type);

        QTableWidgetItem *size = new QTableWidgetItem(file.size);
        ui->measurementTable->setItem(row, 3, size);

        row++;
    }
}

void MeasurementTable::enableMeasureButton()
{
    ui->measureButton->setEnabled(true);
}

void MeasurementTable::disableMeasureButton()
{
    ui->measureButton->setEnabled(false);
}

void MeasurementTable::hideMeasureButton()
{
    ui->measureButton->hide();
}

void MeasurementTable::showMeasureButton()
{
    ui->measureButton->show();
}

void MeasurementTable::enableFinishButton()
{
    ui->submitButton->setEnabled(true);
}

void MeasurementTable::disableFinishButton()
{
    ui->submitButton->setEnabled(false);
}

void MeasurementTable::hideManualEntry()
{
    ui->manualEntryToggle->hide();
    ui->manualEntryToggle->setEnabled(false);
}

void MeasurementTable::showManualEntry()
{
    ui->manualEntryToggle->hide();
    ui->manualEntryToggle->setEnabled(false);
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

    qDebug() << "enter manual entry mode" << manualEntryMode;

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
    } else {
        if (saveChanges)
        {
            qDebug() << "save manual changes";
            saveManualChanges();
        }

        qDebug() << "here";

        ui->manualEntryToggle->setText("Manual Entry");
        ui->addMeasureButton->setVisible(false);
        ui->measureButton->setVisible(true);

        ui->measurementTable->removeColumn(ui->measurementTable->columnCount() - 1);
        ui->measurementTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        ui->measurementTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}


