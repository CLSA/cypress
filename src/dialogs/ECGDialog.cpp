#include "ECGDialog.h"
#include "managers/ecg/ECGManager.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

ECGDialog::ECGDialog(QWidget *parent)
    : DialogBase(parent)
    , ui(new Ui::RunnableDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new ECGManager(this));
    this->setWindowTitle("ECG");
}

ECGDialog::~ECGDialog()
{
    delete ui;
}

void ECGDialog::initializeModel()
{
    m_manager.get()->initializeModel();
    ui->measureWidget->initialize(m_manager.get()->getModel());
}

// set up signal slot connections between GUI front end
// and device management back end
//
void ECGDialog::initializeConnections()
{
  QSharedPointer<ECGManager> derived = m_manager.staticCast<ECGManager>();

  // Disable all buttons by default
  //
  foreach(auto button, this->findChildren<QPushButton *>())
  {
      if("Close" != button->text())
        button->setEnabled(false);

      // disable enter key press event passing onto auto focus buttons
      //
      button->setDefault(false);
      button->setAutoDefault(false);
  }

  // Relay messages from the manager to the status bar
  //
  connect(m_manager.get(),&ManagerBase::message,
          ui->statusBar, &QStatusBar::showMessage, Qt::DirectConnection);

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
  if(Constants::RunMode::modeSimulate == m_mode)
  {
    ui->barcodeWidget->setBarcode(Constants::DefaultBarcode);
  }

  connect(ui->barcodeWidget,&BarcodeWidget::validated,
          this,[this](const bool& valid)
    {
      if(valid)
      {
          // launch the manager
          //
          this->run();
      }
      else
      {
          QMessageBox::critical(
            this, QApplication::applicationName(),
            tr("The input does not match the expected barcode for this participant."));
      }
  });

    connect(derived.get(),&ECGManager::canSelectRunnable,
            this,[this](){
        foreach(auto button, this->findChildren<QPushButton *>())
        {
          if("Close" != button->text())
            button->setEnabled(false);
        }
        ui->openButton->setEnabled(true);
        static bool warn = true;
        if(warn)
        {
            QMessageBox::warning(
                        this, QApplication::applicationName(),
                        tr("Select the exe by clicking Open and browsing to the "
                        "required executable (Cardio.exe) and selecting the file.  If the executable "
                        "is valid click the Run button to start the test otherwise check the installation."));
            warn = false;
        }
    });

    connect(ui->openButton, &QPushButton::clicked,
            derived.get(), &ECGManager::select);

    connect(derived.get(),&ECGManager::canSelectWorking,
            this,[this](){
        foreach(auto button, this->findChildren<QPushButton *>())
        {
          if("Close" != button->text())
            button->setEnabled(false);
        }
        ui->openButton->setEnabled(true);
        static bool warn = true;
        if(warn)
        {
            QMessageBox::warning(
            this, QApplication::applicationName(),
            tr("Select the path to the Cardiosoft database directory by clicking Open and browsing to "
            "and selecting the folder (C:/CARDIO).  If the folder "
            "is valid click the Run button to start the test otherwise check the installation."));
            warn = false;
        }
    });

    // Available to start measuring
    //
    connect(derived.get(), &ECGManager::canMeasure,
            ui->measureWidget, &MeasureWidget::enableMeasure);

    // Request a measurement from the device
    //
    connect(ui->measureWidget, &MeasureWidget::measure,
        derived.get(), &ECGManager::measure);

    // Update the UI with any data
    //
    connect(derived.get(), &ECGManager::dataChanged,
        ui->measureWidget, &MeasureWidget::updateModelView);

    // All measurements received: enable write test results
    //
    connect(derived.get(), &ECGManager::canWrite,
        ui->measureWidget, &MeasureWidget::enableWriteToFile);

    // Write test data to output
    //
    connect(ui->measureWidget, &MeasureWidget::writeToFile,
        this, &DialogBase::writeOutput);

    // Close the application
    //
    connect(ui->measureWidget, &MeasureWidget::closeApplication,
        this, &DialogBase::close);
}

QString ECGDialog::getVerificationBarcode() const
{
  return ui->barcodeWidget->barcode();
}

void ECGDialog::setVerificationBarcode(const QString &barcode)
{
    ui->barcodeWidget->setBarcode(barcode);
}
