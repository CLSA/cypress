#include "spirometer_dialog.h"
#include "managers/spirometer/spirometer_manager.h"
#include "cypress_application.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

SpirometerDialog::SpirometerDialog(QWidget* parent, const CypressSession& session):
    DialogBase(parent, session),
    ui(new Ui::SpirometerDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new SpirometerManager(session));

    SpirometerManager* manager = static_cast<SpirometerManager*>(m_manager.get());

    ui->testInfoWidget->setSessionInformation(session);

    QList<TableColumn> columns;

    // device started
    connect(manager, &SpirometerManager::started, ui->measurementTable, [=](TestBase *test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager, &SpirometerManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // auto measure
    connect(manager, &SpirometerManager::measured, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // can finish
    connect(manager, &SpirometerManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager, &SpirometerManager::success, this, &SpirometerDialog::success);

    // critical error
    connect(manager, &SpirometerManager::error, this, &SpirometerDialog::error);

    // data changed
    connect(manager, &SpirometerManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &SpirometerManager::measure);

    connect(ui->measurementTable, &MeasurementTable::enterManualEntry, manager, [=]() {
        manager->setManualEntry(true);
    });

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &SpirometerManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager, &SpirometerManager::addManualMeasurement);
}

SpirometerDialog::~SpirometerDialog()
{
    delete ui;
}

// set up signal slot connections between GUI front end
// and device management back end
//
void SpirometerDialog::initializeConnections()
{
  //QSharedPointer<SpirometerManager> derived =
  //  m_manager.staticCast<SpirometerManager>();

  //  if (Cypress::getInstance().isSimulation()) {
  //    //ui->measureWidget->enableMeasure();

  //} else {
  //    // Disable all buttons by default
  ////
  //foreach(auto button, this->findChildren<QPushButton *>())
  //{
  //    if("Close" != button->text())
  //      button->setEnabled(false);

  //    // disable enter key press event passing onto auto focus buttons
  //    //
  //    button->setDefault(false);
  //    button->setAutoDefault(false);
  //}

  //  }


  //// Relay messages from the manager to the status bar
  ////
  ////connect(m_manager.get(),&ManagerBase::message,
  ////        ui->statusBar, &QStatusBar::showMessage, Qt::DirectConnection);

  //// Every instrument stage launched by an interviewer requires input
  //// of the interview barcode that accompanies a participant.
  //// The expected barcode is passed from upstream via .json file.
  //// In simulate mode this value is ignored and a default barcode Constants::DefaultBarcode is
  //// assigned instead.
  //// In production mode the input to the barcodeLineEdit is verified against
  //// the content held by the manager and a message or exception is thrown accordingly
  ////
  //// TODO: for DCS interviews, the first digit corresponds the the wave rank
  //// for inhome interviews there is a host dependent prefix before the barcode
  ////
  //  connect(derived.get(), &SpirometerManager::canSelectRunnable,
  //          this, [this]() {
  //      foreach(auto button, this->findChildren<QPushButton *>())
  //      {
  //        if("Close" != button->text())
  //          button->setEnabled(false);
  //      }
  //      ui->openButton->setEnabled(true);
  //      static bool warn = true;
  //      if(warn)
  //      {
  //        QMessageBox::warning(
  //          this, QApplication::applicationName(),
  //          tr("Select the exe by clicking Open and browsing to the "
  //             "required executable (EasyWarePro.exe) and selecting the file.  If the executable "
  //             "is valid click the Run button to start the test otherwise check the installation."));
  //        warn = false;
  //      }
  //  });

  //  connect(ui->openButton, &QPushButton::clicked,
  //          derived.get(), &SpirometerManager::select);

  //  connect(derived.get(), &SpirometerManager::canSelectDataPath,
  //          this, [this]() {
  //      foreach(auto button, this->findChildren<QPushButton *>())
  //      {
  //        if("Close" != button->text())
  //          button->setEnabled(false);
  //      }
  //      ui->openButton->setEnabled(true);
  //      static bool warn = true;
  //      if(warn)
  //      {
  //        QMessageBox::warning(
  //          this, QApplication::applicationName(),
  //          tr("Select the EMR transfer directory by clicking Open and browsing to the "
  //             "required directory (likely C:/ProgramData/ndd/Easy on-PC) and selecting the it. "
  //             "Click the Run button to start the test otherwise check the installation."));
  //        warn = false;
  //      }
  //  });

  //  // Available to start measuring
  //  //
  //  //connect(derived.get(), &SpirometerManager::canMeasure,
  //  //        ui->measureWidget, &MeasureWidget::enableMeasure);

  //  // Request a measurement from the device
  //  //
  //  //connect(ui->measureWidget, &MeasureWidget::measure,
  //  //    derived.get(), &SpirometerManager::measure);

  //  // Update the UI with any data
  //  //
  //  //connect(derived.get(), &SpirometerManager::dataChanged,
  //  //    ui->measureWidget, &MeasureWidget::updateModelView);

  //  // All measurements received: enable write test results
  //  //
  //  //connect(derived.get(), &SpirometerManager::canFinish,
  //  //    ui->measureWidget, &MeasureWidget::enableWriteToFile);

  //  // Close the application
  //  //
  //  //connect(ui->measureWidget, &MeasureWidget::closeApplication,
  //  //    this, &DialogBase::close);
}
