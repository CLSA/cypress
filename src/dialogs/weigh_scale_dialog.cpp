#include "weigh_scale_dialog.h"
#include "managers/weigh_scale/weigh_scale_manager.h"

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

WeighScaleDialog::WeighScaleDialog(QWidget *parent, QSharedPointer<WeighScaleSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::WeighScaleDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new WeighScaleManager(session));

    QSharedPointer<WeighScaleManager> manager = qSharedPointerCast<WeighScaleManager>(m_manager);
    weightManualEntryForm = new WeightScaleManualEntryForm(this);

    ui->testInfoWidget->setSessionInformation(*session);
    ui->measurementTable->disableMeasureButton();

    QList<TableColumn> columns;
    columns << TableColumn("weight", "Weight", new NumberDelegate(0, 1000, true, false, false, 2));
    columns << TableColumn("mode", "Mode", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("timestamp", "Time", new TextDelegate("", QRegExp(), false));

    // port picker setup
    // ports have been scanned
    connect(manager.get(), &WeighScaleManager::scanningDevices, ui->serialPortPickerWidget, &SerialPortWidget::scanningForDevices);

    // the default port in the .ini file was found, update the UI
    connect(manager.get(), &WeighScaleManager::deviceSelected, ui->serialPortPickerWidget, &SerialPortWidget::deviceFound);

    // ports have been scanned
    connect(manager.get(), &WeighScaleManager::devicesDiscovered, ui->serialPortPickerWidget, &SerialPortWidget::devicesDiscovered);

    // ui should now allow picking a device
    connect(manager.get(), &WeighScaleManager::canSelectDevice, ui->serialPortPickerWidget, &SerialPortWidget::devicesCanBeSelected);

    // the user selected a device from the dropdown
    connect(ui->serialPortPickerWidget, &SerialPortWidget::deviceSelected, manager.get(), &WeighScaleManager::selectDevice);

    // the selected device is valid for this test
    connect(manager.get(), &WeighScaleManager::canConnectDevice, ui->serialPortPickerWidget, &SerialPortWidget::canConnectDevice);

    // the user requested to connect to the device
    connect(ui->serialPortPickerWidget, &SerialPortWidget::connectDevice, manager.get(), &WeighScaleManager::connectDevice);

    // the user requested to disconnect from the device
    connect(ui->serialPortPickerWidget, &SerialPortWidget::disconnectDevice, manager.get(), [=]() {
        ui->serialPortPickerWidget->deviceDisconnected();
        ui->zeroDevicePushButton->setEnabled(false);
    });

    // a connection was made, update ui
    connect(manager.get(), &WeighScaleManager::deviceConnected, ui->serialPortPickerWidget, [=](const QSerialPortInfo& portInfo) {
        ui->serialPortPickerWidget->deviceConnected(portInfo);
        ui->zeroDevicePushButton->setEnabled(true);
        ui->measurementTable->enableMeasureButton();
    });

    // device disconnected, update ui
    connect(manager.get(), &WeighScaleManager::deviceDisconnected, ui->serialPortPickerWidget, [=]() {
        ui->serialPortPickerWidget->disconnectDevice();
        ui->zeroDevicePushButton->setEnabled(false);
        ui->measurementTable->disableMeasureButton();
    });

    // device started
    connect(manager.get(), &WeighScaleManager::started, ui->measurementTable, [=](QSharedPointer<TestBase> test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager.get(), &WeighScaleManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // cannot auto measure
    connect(manager.get(), &WeighScaleManager::cannotMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->disableMeasureButton();
    });

    // can finish
    connect(manager.get(), &WeighScaleManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager.get(), &WeighScaleManager::success, this, &WeighScaleDialog::success);

    // critical error
    connect(manager.get(), &WeighScaleManager::error, this, &WeighScaleDialog::error);

    // data changed
    connect(manager.get(), &WeighScaleManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager.get(), &WeighScaleManager::measure);

    connect(ui->measurementTable, &MeasurementTable::enterManualEntry, manager.get(), [=]() {
        manager->setManualEntry(true);

        weightManualEntryForm->clearForm();
        weightManualEntryForm->show();
    });

    connect(weightManualEntryForm, &WeightScaleManualEntryForm::manualWeightEntry, this, [=](const double weight) {
        manager->addManualEntry(weight);
    });

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager.get(), &WeighScaleManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager.get(), &WeighScaleManager::addManualMeasurement);

    // zero device
    connect(ui->zeroDevicePushButton, &QPushButton::clicked, manager.get(), &WeighScaleManager::zeroDevice);
}

WeighScaleDialog::~WeighScaleDialog()
{
    delete ui;
}
