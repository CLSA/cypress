#include "audiometer_dialog.h"
#include "managers/audiometer/audiometer_manager.h"

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

AudiometerDialog::AudiometerDialog(QWidget* parent, const CypressSession& session):
    DialogBase(parent, session),
    ui(new Ui::AudiometerDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);
    m_manager.reset(new AudiometerManager(session));

    AudiometerManager* manager = static_cast<AudiometerManager*>(m_manager.get());

    ui->testInfoWidget->setSessionInformation(session);

    QList<TableColumn> columns;
    columns << TableColumn("side", "Side", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("test", "Test", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("level", "Level", new TextDelegate("", QRegExp(), true));

    // device started
    connect(manager, &AudiometerManager::started, ui->measurementTable, [=](TestBase* test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager, &AudiometerManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // auto measure
    connect(manager, &AudiometerManager::measured, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // can finish
    connect(manager, &AudiometerManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager, &AudiometerManager::success, this, &AudiometerDialog::success);

    // critical error
    connect(manager, &AudiometerManager::error, this, &AudiometerDialog::error);

    // data changed
    connect(manager, &AudiometerManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &AudiometerManager::measure);

    connect(ui->measurementTable, &MeasurementTable::enterManualEntry, manager, [=]() {
        manager->setManualEntry(true);
    });

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &AudiometerManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager, &AudiometerManager::addManualMeasurement);
}

AudiometerDialog::~AudiometerDialog()
{
    delete ui;
}

// set up signal slot connections between GUI front end
// and device management back end
//
void AudiometerDialog::initializeConnections()
{
    //QSharedPointer<AudiometerManager> audiometerManager = m_manager.staticCast<AudiometerManager>(); // inherits from SerialPortManager

    //// Scan for devices
    //connect(audiometerManager.get(), &SerialPortManager::scanningDevices, ui->serialPortWidget, &SerialPortWidget::clear);

    //// Update the drop down list as devices are discovered during scanning
    //connect(audiometerManager.get(), &SerialPortManager::devicesDiscovered, ui->serialPortWidget, &SerialPortWidget::devicesDiscovered);
    //connect(ui->serialPortWidget, &SerialPortWidget::deviceSelected, audiometerManager.get(), &SerialPortManager::deviceSelected);

    // Ready to connect device
    //connect(audiometerManager.get(), &SerialPortManager::canConnectDevice,
    //  this,[this](){
    //    ui->connectButton->setEnabled(true);
    //    ui->disconnectButton->setEnabled(false);
    //});

    // Connect to device
    //connect(ui->connectButton, &QPushButton::clicked, audiometerManager.get(), &AudiometerManager::connectDevice);

    //// Connection is established: enable measurement requests
    //connect(audiometerManager.get(), &AudiometerManager::canMeasure,
    //        ui->measurements, &MeasurementTable::enableMeasureButton);

    //// allow disconnecting from the device
    //connect(audiometerManager.get(), &AudiometerManager::canMeasure,
    //          this, [this](){
    //  ui->connectButton->setEnabled(false);
    //  ui->disconnectButton->setEnabled(true);
    //});

    //// Disconnect from device
    //connect(ui->disconnectButton, &QPushButton::clicked, audiometerManager.get(), &AudiometerManager::disconnectDevice);

//  // Request a measurement from the device
//  //
//  //connect(ui->measureWidget, &MeasureWidget::measure,
//  //      derived.get(), &AudiometerManager::measure);
//
//  // Update the UI with any data
//  //
//  //connect(derived.get(), &AudiometerManager::dataChanged,
//  //    ui->measureWidget, &MeasureWidget::updateModelView);
//
//  // All measurements received: enable write test results
//  //
//  //connect(derived.get(), &AudiometerManager::canFinish,
//  //    ui->measureWidget, &MeasureWidget::enableWriteToFile);
//
//  // Close the application
//  //
//  //connect(ui->measureWidget, &MeasureWidget::closeApplication,
//  //    this, &DialogBase::close);
}
