#include "audiometer_dialog.h"
#include "managers/audiometer/audiometer_manager.h"

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

AudiometerDialog::AudiometerDialog(QWidget *parent, QSharedPointer<AudiometerSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::AudiometerDialog)
{
    ui->setupUi(this);
    ui->measurementTable->disableFinishButton();
    ui->measurementTable->disableMeasureButton();
    ui->testInfoWidget->setSessionInformation(*session, "RA300");
    ui->measurementTable->enableRemoval(true);

    audiometerManualEntryForm = new AudiometerManualEntryForm(this);

    m_manager.reset(new AudiometerManager(session));
    auto manager = qSharedPointerCast<AudiometerManager>(m_manager);

    QList<TableColumn> columns;
    columns << TableColumn("side", "Side", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("test", "Test", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("level", "Level", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("outcome", "Outcome", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("error", "Error", new TextDelegate("", QRegExp(), true));

    connect(manager.get(), &AudiometerManager::scanningDevices, ui->serialPortPickerWidget, &SerialPortWidget::scanningForDevices);

    // ports have been scanned
    connect(manager.get(), &AudiometerManager::devicesDiscovered, ui->serialPortPickerWidget, &SerialPortWidget::devicesDiscovered);

    // ui should now allow picking a device
    connect(manager.get(), &AudiometerManager::canSelectDevice, ui->serialPortPickerWidget, &SerialPortWidget::devicesCanBeSelected);

    // the default port in the .ini file was found, update the UI
    connect(manager.get(), &AudiometerManager::deviceSelected, ui->serialPortPickerWidget, &SerialPortWidget::deviceFound);

    // the user selected a device from the dropdown
    connect(ui->serialPortPickerWidget, &SerialPortWidget::deviceSelected, manager.get(), &AudiometerManager::selectDevice);

    // the selected device is valid for this test
    connect(manager.get(), &AudiometerManager::canConnectDevice, ui->serialPortPickerWidget, &SerialPortWidget::canConnectDevice);

    // the user requested to connect to the device
    connect(ui->serialPortPickerWidget, &SerialPortWidget::connectDevice, manager.get(), &AudiometerManager::connectDevice);

    // the user requested to disconnect from the device
    connect(ui->serialPortPickerWidget, &SerialPortWidget::disconnectDevice, manager.get(), &AudiometerManager::disconnectDevice);

    // a connection was made, update ui
    connect(manager.get(), &AudiometerManager::deviceConnected, ui->serialPortPickerWidget, [=](const QSerialPortInfo& portInfo) {
        ui->serialPortPickerWidget->deviceConnected(portInfo);
        ui->measurementTable->enableMeasureButton();
    });

    // device disconnected, update ui
    connect(manager.get(), &AudiometerManager::deviceDisconnected, ui->serialPortPickerWidget, [=]() {
        ui->serialPortPickerWidget->deviceDisconnected();
        ui->measurementTable->disableMeasureButton();
    });
    // device started
    connect(manager.get(), &AudiometerManager::started, ui->measurementTable, [=](QSharedPointer<TestBase> test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager.get(), &AudiometerManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // can finish
    connect(manager.get(), &AudiometerManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager.get(), &AudiometerManager::success, this, &AudiometerDialog::success);

    // critical error
    connect(manager.get(), &AudiometerManager::error, this, &AudiometerDialog::error);

    // data changed
    connect(manager.get(), &AudiometerManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);


    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager.get(), &AudiometerManager::measure);

    connect(ui->measurementTable, &MeasurementTable::enterManualEntry, manager.get(), [=]() {
        manager->setManualEntry(true);

        audiometerManualEntryForm->clearForm();
        audiometerManualEntryForm->show();
    });

    connect(audiometerManualEntryForm, &AudiometerManualEntryForm::manualAudiometerMeasurement, this, [=](const QString side, const QString test, const int level) {
        manager->addManualEntry(side, test, level, level <= 40);
    });

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager.get(), &AudiometerManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager.get(), &AudiometerManager::addManualMeasurement);
}

AudiometerDialog::~AudiometerDialog()
{
    delete ui;
}
