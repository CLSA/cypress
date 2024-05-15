#include "blood_pressure_dialog.h"
#include "managers/blood_pressure/blood_pressure_manager.h"

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

BloodPressureDialog::BloodPressureDialog(QWidget *parent, QSharedPointer<BPMSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::BloodPressureDialog)
{
    ui->setupUi(this);

    m_manager.reset(new BloodPressureManager(session));
    auto manager = qSharedPointerCast<BloodPressureManager>(m_manager);

    ui->testInfoWidget->setSessionInformation(*session);
    ui->measurementTable->hideMeasureButton();
    ui->measurementTable->enableRemoval(true);

    ui->connectPushButton->setEnabled(false);

    bpmManualEntryForm = new BpmManualEntryForm(this);

    QList<TableColumn> columns;
    columns << TableColumn("reading_number",
                           "#",
                           new NumberDelegate(0, 1000, true, false, false, 2));
    columns << TableColumn("systolic", "Systolic (mmHg)", new NumberDelegate(0, 200, false));
    columns << TableColumn("diastolic", "Diastolic (mmHg)", new NumberDelegate(0, 200, false));
    columns << TableColumn("pulse", "Pulse (bpm)", new NumberDelegate(0, 200, false));
    columns << TableColumn("start_time", "Start time", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("end_time", "End time", new TextDelegate("", QRegExp(), true));

    QStringList bandList = {"---", "Small", "Medium", "Large", "X-Large"};
    ui->armBandSizeComboBox->addItems(bandList);
    connect(ui->armBandSizeComboBox, &QComboBox::currentTextChanged, manager.get(), &BloodPressureManager::setCuffSize);

    QStringList armList = {"---", "Left", "Right"};
    ui->armComboBox->addItems(armList);
    connect(ui->armComboBox, &QComboBox::currentTextChanged, manager.get(), &BloodPressureManager::setSide);

    connect(manager.get(), &BloodPressureManager::canConnect, this, [=]() {
        ui->connectPushButton->setEnabled(true);
    });

    connect(manager.get(), &BloodPressureManager::deviceStateChanged, this, [=](QString state) {
        ui->stateValue->setText(state);
    });

    connect(ui->connectPushButton, &QPushButton::clicked, manager.get(), [=]() {
      if (ui->armComboBox->currentText() == "---") {
        QMessageBox::warning(this, "Select Arm",
                             "Please select the arm used from the dropdown");
        return;
      }

      if (ui->armBandSizeComboBox->currentText() == "---") {
        QMessageBox::warning(
            this, "Select Cuff Size",
            "Please select the cuff size used from the dropdown");
        return;
      }

      manager->connectToDevice();
    });

    connect(ui->disconnectPushButton,
            &QPushButton::clicked,
            manager.get(),
            &BloodPressureManager::disconnectFromDevice);

    connect(ui->cyclePushButton, &QPushButton::clicked, manager.get(), &BloodPressureManager::cycle);
    connect(ui->clearPushButton, &QPushButton::clicked, manager.get(), &BloodPressureManager::clear);

    connect(ui->startPushButton,
            &QPushButton::clicked,
            manager.get(),
            &BloodPressureManager::startMeasurement);

    connect(ui->stopPushButton,
            &QPushButton::clicked,
            manager.get(),
            &BloodPressureManager::stopMeasurement);

    connect(manager.get(), &BloodPressureManager::deviceHandshaked, this, [=](QString firmware) {
        qDebug() << "handshaked, setting controls";

        ui->connectPushButton->setEnabled(false);
        ui->disconnectPushButton->setEnabled(true);

        ui->cyclePushButton->setEnabled(true);
        ui->clearPushButton->setEnabled(true);
        ui->startPushButton->setEnabled(true);
    });

    connect(manager.get(), &BloodPressureManager::deviceCycled, this, [=](QString cycle) {
        ui->cycleValue->setText(cycle);
    });

    connect(manager.get(), &BloodPressureManager::inflateCuffPressure, this, [=](int cuffPressure) {
        ui->cuffPressureValue->setText(QString::number(cuffPressure));
    });

    connect(manager.get(), &BloodPressureManager::deflateCuffPressure, this, [=](int cuffPressure) {
        ui->cuffPressureValue->setText(QString::number(cuffPressure));
    });

    connect(manager.get(), &BloodPressureManager::deviceCleared, this, [=]() {
        ui->readingValue->setText("");
    });

    connect(manager.get(), &BloodPressureManager::deviceDisconnected, this, [=]() {
        ui->connectPushButton->setEnabled(true);

        ui->disconnectPushButton->setEnabled(false);
        ui->startPushButton->setEnabled(false);
        ui->stopPushButton->setEnabled(false);
        ui->cyclePushButton->setEnabled(false);
        ui->clearPushButton->setEnabled(false);
    });

    connect(manager.get(), &BloodPressureManager::measurementStarted, this, [=]() {
        ui->stopPushButton->setEnabled(true);
        ui->startPushButton->setEnabled(false);

        ui->clearPushButton->setEnabled(false);
        ui->cyclePushButton->setEnabled(false);
        ui->disconnectPushButton->setEnabled(false);
    });

    connect(manager.get(), &BloodPressureManager::measurementStopped, this, [=]() {
        ui->stopPushButton->setEnabled(false);
        ui->startPushButton->setEnabled(true);

        ui->clearPushButton->setEnabled(true);
        ui->cyclePushButton->setEnabled(true);
        ui->disconnectPushButton->setEnabled(true);
    });

    connect(manager.get(), &BloodPressureManager::started, ui->measurementTable, [=](QSharedPointer<TestBase> test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    connect(manager.get(), &BloodPressureManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });
    connect(manager.get(), &BloodPressureManager::cannotMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->disableMeasureButton();
    });

    connect(manager.get(), &BloodPressureManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });
    connect(manager.get(), &BloodPressureManager::cannotFinish, ui->measurementTable, [=]() {
        ui->measurementTable->disableFinishButton();
    });

    connect(manager.get(), &BloodPressureManager::success, this, &BloodPressureDialog::success);
    connect(manager.get(), &BloodPressureManager::error, this, &BloodPressureDialog::error);
    connect(manager.get(), &BloodPressureManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager.get(), &BloodPressureManager::measure);

    connect(ui->measurementTable, &MeasurementTable::enterManualEntry, manager.get(), [=]() {
        if (ui->armComboBox->currentText() == "---") {
            QMessageBox::warning(this, "Select Arm",
                         "Please select the arm used from the dropdown");
            return;
        }

        if (ui->armBandSizeComboBox->currentText() == "---") {
            QMessageBox::warning(
                this, "Select Cuff Size",
                "Please select the cuff size used from the dropdown");
            return;
        }

        manager->setManualEntry(true);

        bpmManualEntryForm->clearForm();
        bpmManualEntryForm->show();
    });

    connect(bpmManualEntryForm, &BpmManualEntryForm::manualBpmMeasure, this, [=](const int systolic, const int diastolic, const int pulse) {
        manager->addManualEntry(systolic, diastolic, pulse);
    });

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager.get(), &BloodPressureManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager.get(), &BloodPressureManager::addManualMeasurement);
    connect(ui->measurementTable, &MeasurementTable::removeMeasurement, manager.get(), &BloodPressureManager::removeMeasurement);
}

BloodPressureDialog::~BloodPressureDialog()
{
    qDebug() << "destroy blood pressure dialog";
    delete ui;
}
