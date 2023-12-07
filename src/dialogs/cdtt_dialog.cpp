#include "cdtt_dialog.h"
#include "managers/cdtt/cdtt_manager.h"

#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

CDTTDialog::CDTTDialog(QWidget *parent, QSharedPointer<CDTTSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::CDTTDialog)
{
    ui->setupUi(this);
    ui->measurementTable->disableMeasureButton();
    ui->measurementTable->disableFinishButton();
    ui->measurementTable->hideMeasureButton();
    ui->measurementTable->hideManualEntry();

    this->setWindowTitle("FRAX");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new CDTTManager(session));

    CDTTManager* manager = static_cast<CDTTManager*>(m_manager.get());

    ui->testInfoWidget->setSessionInformation(*session);

    QList<TableColumn> columns;

    columns << TableColumn("TYPE", "Type", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("PROBABILITY", "Probability", new TextDelegate("", QRegExp(), true));

    // device started
    connect(manager, &CDTTManager::started, ui->measurementTable, [=](TestBase* test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager, &CDTTManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // can finish
    connect(manager, &CDTTManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager, &CDTTManager::success, this, &CDTTDialog::success);

    // critical error
    connect(manager, &CDTTManager::error, this, &CDTTDialog::error);

    // data changed
    connect(manager, &CDTTManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &CDTTManager::measure);

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &CDTTManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager, &CDTTManager::addManualMeasurement);
}

CDTTDialog::~CDTTDialog()
{
    delete ui;
}

// set up signal slot connections between GUI front end
// and device management back end
//
void CDTTDialog::initializeConnections()
{
  QSharedPointer<CDTTManager> derived = m_manager.staticCast<CDTTManager>();

  // Disable all buttons by default
  //

  //if (Cypress::getInstance().isSimulation()) {
  //    ui->measureWidget->enableMeasure();
  //}
  //else {
  //    foreach(auto button, this->findChildren<QPushButton *>())
  //    {
  //      if("Close" != button->text())
  //          button->setEnabled(false);

  //      // disable enter key press event passing onto auto focus buttons
  //      //
  //      button->setDefault(false);
  //      button->setAutoDefault(false);
  //    }
  //}


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
  //if(Constants::RunMode::modeSimulate == m_mode)
  //{
  //  ui->barcodeWidget->setBarcode(Constants::DefaultBarcode);
  //}

  //connect(ui->barcodeWidget,&BarcodeWidget::validated,
  //        this,[this](const bool& valid)
  //  {
  //    if(valid)
  //    {
  //        // launch the manager
  //        //
  //        this->run();
  //    }
  //    else
  //    {
  //        QMessageBox::critical(
  //          this, QApplication::applicationName(),
  //          tr("The input does not match the expected barcode for this participant."));
  //    }
  //});

  //connect(derived.get(),&CDTTManager::canSelectRunnable,
  //          this,[this](){
  //      foreach(auto button, this->findChildren<QPushButton *>())
  //      {
  //        if("Close" != button->text())
  //          button->setEnabled(false);
  //      }
  //      ui->openButton->setEnabled(true);
  //      static bool warn = true;
  //      if(warn)
  //      {
  //          QMessageBox::warning(
  //          this, QApplication::applicationName(),
  //          tr("Select the exe by clicking Open and browsing to the "
  //          "required executable (CDTTStereo.jar) and selecting the file.  If the executable "
  //          "is valid click the Run button to start the test otherwise check the installation."));
  //          warn = false;
  //      }
  //  });

  //connect(ui->openButton, &QPushButton::clicked,
  //      this, [this, derived]() {
  //          QString fileName =
  //              QFileDialog::getOpenFileName(
  //                  this, tr("Open File"),
  //                  QCoreApplication::applicationDirPath(),
  //                  tr("Applications (*.jar, *)"));

  //          derived->selectRunnable(fileName);
  //      });

  // Available to start measuring
  //
  //connect(derived.get(), &CDTTManager::canMeasure,
  //        ui->measureWidget, &MeasureWidget::enableMeasure);

  // Request a measurement from the device
  //
  //connect(ui->measureWidget, &MeasureWidget::measure,
  //    derived.get(), &CDTTManager::measure);

  // Update the UI with any data
  //
  //connect(derived.get(), &CDTTManager::dataChanged,
  //    ui->measureWidget, &MeasureWidget::updateModelView);

  // All measurements received: enable write test results
  //
  //connect(derived.get(), &CDTTManager::canFinish,
  //    ui->measureWidget, &MeasureWidget::enableWriteToFile);

  // Close the application
  //
  //connect(ui->measureWidget, &MeasureWidget::closeApplication,
  //    this, &DialogBase::close);
}
