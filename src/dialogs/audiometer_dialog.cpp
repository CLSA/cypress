#include "audiometer_dialog.h"
#include "managers/audiometer/audiometer_manager.h"
#include "widgets/serial_port_widget.h"
#include "cypress_application.h"

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

AudiometerDialog::AudiometerDialog(QWidget* parent, const CypressSession& session):
    DialogBase(parent, session),
    ui(new Ui::AudiometerDialog)
{
    qDebug() << "AudiometerDialog::AudiometerDialog()";
    m_manager.reset(new AudiometerManager(m_session));

    ui->setupUi(this);
    ui->testInfoWidget->setSessionInformation(session);
    ui->testInfoWidget->setDeviceStatus("Not connected");

    setWindowFlags(Qt::WindowFullscreenButtonHint);
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
    QSharedPointer<AudiometerManager> audiometerManager = m_manager.staticCast<AudiometerManager>(); // inherits from SerialPortManager

    // Scan for devices
    connect(audiometerManager.get(), &SerialPortManager::scanningDevices, ui->serialPortWidget, &SerialPortWidget::clear);

    // Update the drop down list as devices are discovered during scanning
    connect(audiometerManager.get(), &SerialPortManager::devicesDiscovered, ui->serialPortWidget, &SerialPortWidget::devicesDiscovered);
    connect(ui->serialPortWidget, &SerialPortWidget::deviceSelected, audiometerManager.get(), &SerialPortManager::deviceSelected);

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
