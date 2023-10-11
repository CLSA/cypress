#include "frax_dialog.h"
#include "managers/frax/frax_manager.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

FraxDialog::FraxDialog(QWidget* parent, const CypressSession& session):
    DialogBase(parent, session),
    ui(new Ui::FraxDialog)
{
    ui->setupUi(this);
    ui->measurementTable->disableMeasureButton();
    ui->measurementTable->disableFinishButton();

    this->setWindowTitle("FRAX");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new FraxManager(session));

    FraxManager* manager = static_cast<FraxManager*>(m_manager.get());

    ui->testInfoWidget->setSessionInformation(session);

    QList<TableColumn> columns;

    columns << TableColumn("TYPE", "Type", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("PROBABILITY", "Probability", new TextDelegate("", QRegExp(), true));

    // device started
    connect(manager, &FraxManager::started, ui->measurementTable, [=](TestBase* test) {
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager, &FraxManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // auto measure
    connect(manager, &FraxManager::measured, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // can finish
    connect(manager, &FraxManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager, &FraxManager::success, this, &FraxDialog::success);

    // critical error
    connect(manager, &FraxManager::error, this, &FraxDialog::error);

    // data changed
    connect(manager, &FraxManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &FraxManager::measure);

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &FraxManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager, &FraxManager::addManualMeasurement);
}

FraxDialog::~FraxDialog()
{
    delete ui;
}

// set up signal slot connections between GUI front end
// and device management back end
//
void FraxDialog::initializeConnections()
{
  //QSharedPointer<FraxManager> derived = m_manager.staticCast<FraxManager>();

  //// Disable all buttons by default
  ////

  //if (Cypress::getInstance().isSimulation()) {
  //    //ui->measureWidget->enableMeasure();
  //}
  //else {
  //foreach(auto button, this->findChildren<QPushButton *>())
  //{
  //    if("Close" != button->text())
  //      button->setEnabled(false);

  //    // disable enter key press event passing onto auto focus buttons
  //    //
  //    button->setDefault(false);
  //    button->setAutoDefault(false);
  //}
  //}

  //connect(ui->completeButton, &QPushButton::clicked, this, &FraxDialog::userClose);

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
  //connect(derived.get(),&FraxManager::canSelectRunnable,
  //          this,[this](){
  //      foreach(auto button, this->findChildren<QPushButton *>())
  //      {
  //        if("Close" != button->text())
  //          button->setEnabled(false);
  //      }
  //      //ui->openButton->setEnabled(true);
  //      static bool warn = true;
  //      if(warn)
  //      {
  //          QMessageBox::warning(
  //          this, QApplication::applicationName(),
  //          tr("Select the exe by clicking Open and browsing to the "
  //          "required executable (blackbox.exe) and selecting the file.  If the executable "
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
  //                  tr("Applications (*.exe, *)"));

  //          derived->selectRunnable(fileName);
  //      });

  // Available to start measuring
  //
  //connect(derived.get(), &FraxManager::canMeasure,
  //        ui->measureWidget, &MeasureWidget::enableMeasure);

  // Request a measurement from the device
  //
  //connect(ui->measureWidget, &MeasureWidget::measure,
  //    derived.get(), &FraxManager::measure);

  // Update the UI with any data
  //
  //connect(derived.get(), &FraxManager::dataChanged,
  //    ui->measureWidget, &MeasureWidget::updateModelView);

  // All measurements received: enable write test results
  //
  //connect(derived.get(), &FraxManager::canFinish,
  //    ui->measureWidget, &MeasureWidget::enableWriteToFile);

  // Close the application
  //
  //connect(ui->measureWidget, &MeasureWidget::closeApplication,
  //    this, &DialogBase::close);
}
