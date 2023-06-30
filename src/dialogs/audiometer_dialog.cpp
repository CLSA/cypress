#include "audiometer_dialog.h"
#include "managers/audiometer/audiometer_manager.h"
#include "cypress_application.h"

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

AudiometerDialog::AudiometerDialog(QJsonObject inputData): ui(new Ui::AudiometerDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new AudiometerManager(inputData));
}

AudiometerDialog::~AudiometerDialog()
{
    //delete ui;
}

// set up signal slot connections between GUI front end
// and device management back end
//
void AudiometerDialog::initializeConnections()
{
  QSharedPointer<AudiometerManager> derived = m_manager.staticCast<AudiometerManager>();

  // Disable all buttons by default
  //
  
  if (Cypress::getInstance().isSimulation()) {
      ui->measureWidget->enableMeasure();
  }

  else {
      foreach(auto button, this->findChildren<QPushButton *>())
    {
      if("Close" != button->text())
        button->setEnabled(false);

      // disable enter key press event passing onto auto focus buttons
      //
      button->setDefault(false);
      button->setAutoDefault(false);
  }
  }


  connect(ui->completeButton, &QPushButton::clicked, this, &AudiometerDialog::userClose);
  // Relay messages from the manager to the status bar
  //
  //connect(m_manager.get(),&ManagerBase::message,
  //        ui->statusBar, &QStatusBar::showMessage, Qt::DirectConnection);

  // Every instrument stage launched by an interviewer requires input
  // of the interview barcode that accompanies a participant.
  // The expected barcode is passed from upstream via .json file.
  // In simulate mode this value is ignored and a default barcode Constants::DefaultBarcode is
  // assigned instead.
  // In production mode the input to the barcodeLineEdit is verified against
  // the content held by the manager and a message or exception is thrown accordingly
  //
  // TODO: for DCS interviews, the first digit corresponds the the wave rank
  // for inhome interviews there is a host dependent prefix before the barcode
  //

  // Scan for devices
  //
  connect(derived.get(), &AudiometerManager::scanningDevices,
          ui->deviceComboBox, &QComboBox::clear);

  // Update the drop down list as devices are discovered during scanning
  //
  connect(derived.get(), &AudiometerManager::deviceDiscovered,
          this,[this](const QString &label){
      int index = ui->deviceComboBox->findText(label);
      bool oldState = ui->deviceComboBox->blockSignals(true);
      if(-1 == index)
      {
          ui->deviceComboBox->addItem(label);
      }
      ui->deviceComboBox->blockSignals(oldState);
  });

  connect(derived.get(), &AudiometerManager::deviceSelected,
          this,[this](const QString &label){
      if(label != ui->deviceComboBox->currentText())
      {
          ui->deviceComboBox->setCurrentIndex(ui->deviceComboBox->findText(label));
      }
  });

  // Prompt user to select a device from the drop down list when previously
  // cached device information in the ini file is unavailable or invalid
  //
  connect(derived.get(), &AudiometerManager::canSelectDevice,
          this,[this](){
      QMessageBox::warning(
        this, QApplication::applicationName(),
        tr("Select the port from the list.  If the device "
        "is not in the list, quit the application and check that the port is "
        "working and connect the audiometer to it before running this application."));
  });

  // Select a device (serial port) from drop down list
  //
  connect(ui->deviceComboBox, &QComboBox::currentTextChanged,
          derived.get(),&AudiometerManager::selectDevice);

  // Select a device (serial port) from drop down list
  //
  connect(ui->deviceComboBox, QOverload<int>::of(&QComboBox::activated),
    this,[this, derived](int index){
      derived->selectDevice(ui->deviceComboBox->itemText(index));
  });

  // Ready to connect device
  //
  connect(derived.get(), &AudiometerManager::canConnectDevice,
          this,[this](){
      ui->connectButton->setEnabled(true);
      ui->disconnectButton->setEnabled(false);
      ui->measureWidget->disableMeasure();
  });

  // Connect to device
  //
  connect(ui->connectButton, &QPushButton::clicked,
          derived.get(), &AudiometerManager::connectDevice);

  // Connection is established: enable measurement requests
  //
  connect(derived.get(), &AudiometerManager::canMeasure,
          ui->measureWidget, &MeasureWidget::enableMeasure);

  connect(derived.get(), &AudiometerManager::canMeasure,
          this,[this](){
      ui->connectButton->setEnabled(false);
      ui->disconnectButton->setEnabled(true);
  });

  // Disconnect from device
  //
  connect(ui->disconnectButton, &QPushButton::clicked,
          derived.get(), &AudiometerManager::disconnectDevice);

  // Request a measurement from the device
  //
  connect(ui->measureWidget, &MeasureWidget::measure,
        derived.get(), &AudiometerManager::measure);

  // Update the UI with any data
  //
  connect(derived.get(), &AudiometerManager::dataChanged,
      ui->measureWidget, &MeasureWidget::updateModelView);

  // All measurements received: enable write test results
  //
  connect(derived.get(), &AudiometerManager::canFinish,
      ui->measureWidget, &MeasureWidget::enableWriteToFile);

  // Close the application
  //
  connect(ui->measureWidget, &MeasureWidget::closeApplication,
      this, &DialogBase::close);
}

void AudiometerDialog::userClose()
{
    DialogBase::userClose();
    if (m_user_close)
    {
        m_manager->sendComplete("audiometer", m_manager->m_uuid);
    }
    else
    {
        Cypress::getInstance().deviceDialog = nullptr;
        m_manager->sendCancellation("audiometer", m_manager->m_uuid);
    }
    Cypress::getInstance().forceSessionEnd();
}

void AudiometerDialog::closeEvent(QCloseEvent* event)
{
    event->accept();
}


