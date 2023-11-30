#include "blood_pressure_dialog.h"
#include "managers/blood_pressure/blood_pressure_manager.h"
#include "cypress_application.h"

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

BloodPressureDialog::BloodPressureDialog(QWidget *parent, QSharedPointer<BPMSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::BloodPressureDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowFullscreenButtonHint);
    m_manager.reset(new BloodPressureManager(session));

    BloodPressureManager* manager = static_cast<BloodPressureManager*>(m_manager.get());

    ui->testInfoWidget->setSessionInformation(*session);

    if (!m_sim) {
        ui->measurementTable->hideMeasureButton();
    }

    QStringList bandList = {"Small", "Medium", "Large", "X-Large"};
    ui->armBandSizeComboBox->addItems(bandList);
    connect(ui->armBandSizeComboBox, &QComboBox::currentTextChanged, manager, &BloodPressureManager::setCuffSize);

    QStringList armList = {"Left", "Right"};
    ui->armComboBox->addItems(armList);
    connect(ui->armComboBox, &QComboBox::currentTextChanged, manager, &BloodPressureManager::setSide);

    QList<TableColumn> columns;
    columns << TableColumn("reading_number",
                           "#",
                           new NumberDelegate(0, 1000, true, false, false, 2));
    columns << TableColumn("start_time", "Start time", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("end_time", "End time", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("systolic", "Systolic (mmHg)", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("diastolic", "Diastolic (mmHg)", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("pulse", "Pulse (bpm)", new TextDelegate("", QRegExp(), false));

    connect(ui->connectPushButton,
            &QPushButton::clicked,
            manager,
            &BloodPressureManager::connectToDevice);

    connect(ui->disconnectPushButton,
            &QPushButton::clicked,
            manager,
            &BloodPressureManager::disconnectFromDevice);

    connect(ui->cyclePushButton, &QPushButton::clicked, manager, &BloodPressureManager::cycleDevice);

    connect(ui->clearPushButton, &QPushButton::clicked, manager, &BloodPressureManager::clearDevice);

    connect(ui->startPushButton,
            &QPushButton::clicked,
            manager,
            &BloodPressureManager::startMeasurement);

    connect(ui->startPushButton,
            &QPushButton::clicked,
            manager,
            &BloodPressureManager::stopMeasurement);

    connect(manager, &BloodPressureManager::deviceConnected, this, [=]() {
        ui->disconnectPushButton->setEnabled(true);
        ui->connectPushButton->setEnabled(false);

        ui->cyclePushButton->setEnabled(true);
        ui->clearPushButton->setEnabled(true);

        ui->startPushButton->setEnabled(true);
    });

    connect(manager, &BloodPressureManager::deviceDisconnected, this, [=]() {
        ui->disconnectPushButton->setEnabled(false);
        ui->connectPushButton->setEnabled(true);

        ui->cyclePushButton->setEnabled(false);
        ui->clearPushButton->setEnabled(false);
    });

    connect(manager, &BloodPressureManager::measurementStarted, this, [=]() {
        ui->stopPushButton->setEnabled(true);
        ui->startPushButton->setEnabled(false);
    });

    connect(manager, &BloodPressureManager::measurementStopped, this, [=]() {
        ui->stopPushButton->setEnabled(false);
        ui->startPushButton->setEnabled(true);
    });

    // device started
    connect(manager, &BloodPressureManager::started, ui->measurementTable, [=](TestBase* test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager, &BloodPressureManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // cannot auto measure
    connect(manager, &BloodPressureManager::cannotMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->disableMeasureButton();
    });

    // auto measure
    connect(manager, &BloodPressureManager::measured, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // can finish
    connect(manager, &BloodPressureManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager, &BloodPressureManager::success, this, &BloodPressureDialog::success);

    // critical error
    connect(manager, &BloodPressureManager::error, this, &BloodPressureDialog::error);

    // data changed
    connect(manager, &BloodPressureManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &BloodPressureManager::measure);

    connect(ui->measurementTable, &MeasurementTable::enterManualEntry, manager, [=]() {
        manager->setManualEntry(true);
    });

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &BloodPressureManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager, &BloodPressureManager::addManualMeasurement);
}

BloodPressureDialog::~BloodPressureDialog()
{
    if (CypressSettings::isDebugMode())
    {
        qDebug() << "destroy blood pressure dialog";
    }

    delete ui;
}

void BloodPressureDialog::initializeModel()
{

}

void BloodPressureDialog::initializeConnections()
{
    
}
