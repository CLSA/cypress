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

    WeighScaleManager* manager = static_cast<WeighScaleManager*>(m_manager.get());

    ui->testInfoWidget->setSessionInformation(*session);

    QList<TableColumn> columns;
    columns << TableColumn("WEIGHT", "Weight", new NumberDelegate(0, 1000, true, false, false, 2));
    columns << TableColumn("MODE", "Mode", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("TIMESTAMP", "Timestamp", new TextDelegate("", QRegExp(), false));

    // port picker setup
    // ports have been scanned
    connect(manager, &WeighScaleManager::scanningDevices, ui->serialPortPickerWidget, &SerialPortWidget::scanningForDevices);

    // ports have been scanned
    connect(manager, &WeighScaleManager::devicesDiscovered, ui->serialPortPickerWidget, &SerialPortWidget::devicesDiscovered);

    // ui should now allow picking a device
    connect(manager, &WeighScaleManager::canSelectDevice, ui->serialPortPickerWidget, &SerialPortWidget::devicesCanBeSelected);

    // the user selected a device from the dropdown
    connect(ui->serialPortPickerWidget, &SerialPortWidget::deviceSelected, manager, &WeighScaleManager::selectDevice);

    // the selected device is valid for this test
    connect(manager, &WeighScaleManager::canConnectDevice, ui->serialPortPickerWidget, &SerialPortWidget::canConnectDevice);

    // the user requested to connect to the device
    connect(ui->serialPortPickerWidget, &SerialPortWidget::connectDevice, manager, &WeighScaleManager::connectDevice);

    // the user requested to disconnect from the device
    connect(ui->serialPortPickerWidget, &SerialPortWidget::disconnectDevice, manager, &WeighScaleManager::disconnectDevice);

    // a connection was made, update ui
    connect(manager, &WeighScaleManager::deviceConnected, ui->serialPortPickerWidget, &SerialPortWidget::deviceConnected);

    // device disconnected, update ui
    connect(manager, &WeighScaleManager::deviceDisconnected, ui->serialPortPickerWidget, &SerialPortWidget::deviceDisconnected);

    // device started
    connect(manager, &WeighScaleManager::started, ui->measurementTable, [=](TestBase *test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager, &WeighScaleManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // cannot auto measure
    connect(manager, &WeighScaleManager::cannotMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->disableMeasureButton();
    });

    // can finish
    connect(manager, &WeighScaleManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager, &WeighScaleManager::success, this, &WeighScaleDialog::success);

    // critical error
    connect(manager, &WeighScaleManager::error, this, &WeighScaleDialog::error);

    // data changed
    connect(manager, &WeighScaleManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &WeighScaleManager::measure);

    connect(ui->measurementTable, &MeasurementTable::enterManualEntry, manager, [=]() {
        manager->setManualEntry(true);
    });

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &WeighScaleManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager, &WeighScaleManager::addManualMeasurement);

    // zero device
    connect(ui->zeroDevicePushButton, &QPushButton::clicked, manager, &WeighScaleManager::zeroDevice);
}

WeighScaleDialog::~WeighScaleDialog()
{
    delete ui;
}

void WeighScaleDialog::initializeModel()
{
}

void WeighScaleDialog::initializeConnections() {}
