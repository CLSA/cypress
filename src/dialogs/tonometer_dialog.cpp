#include "tonometer_dialog.h"
#include "managers/tonometer/tonometer_manager.h"
#include "cypress_application.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

TonometerDialog::TonometerDialog(QWidget* parent, const CypressSession& session):
    DialogBase { parent, session },
    ui(new Ui::TonometerDialog)
{
    ui->setupUi(this);
    ui->measurementTable->disableMeasureButton();
    ui->measurementTable->disableFinishButton();

    this->setWindowTitle("Tonometer");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new TonometerManager(session));

    TonometerManager* manager = static_cast<TonometerManager*>(m_manager.get());

    ui->testInfoWidget->setSessionInformation(session);

    QList<TableColumn> columns;

    columns << TableColumn("TYPE", "Type", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("PROBABILITY", "Probability", new TextDelegate("", QRegExp(), true));

    // device started
    connect(manager, &TonometerManager::started, ui->measurementTable, [=](TestBase* test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager, &TonometerManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // auto measure
    connect(manager, &TonometerManager::measured, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // can finish
    connect(manager, &TonometerManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager, &TonometerManager::success, this, &TonometerDialog::success);

    // critical error
    connect(manager, &TonometerManager::error, this, &TonometerDialog::error);

    // data changed
    connect(manager, &TonometerManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &TonometerManager::measure);

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &TonometerManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager, &TonometerManager::addManualMeasurement);
}

TonometerDialog::~TonometerDialog()
{
    delete ui;
}

void TonometerDialog::initializeModel()
{
}

// set up signal slot connections between GUI front end
// and device management back end
//
void TonometerDialog::initializeConnections()
{
  //QSharedPointer<TonometerManager> derived =
  //  m_manager.staticCast<TonometerManager>();

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
    //ui->barcodeWidget->setBarcode(Constants::DefaultBarcode);
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

    //connect(derived.get(),&TonometerManager::canSelectRunnable,
    //        this,[this](){
    //    foreach(auto button, this->findChildren<QPushButton *>())
    //    {
    //      if("Close" != button->text())
    //        button->setEnabled(false);
    //    }
    //    ui->openButton->setEnabled(true);
    //    static bool warn = true;
    //    if(warn)
    //    {
    //        QMessageBox::warning(
    //        this, QApplication::applicationName(),
    //        tr("Select the exe by clicking Open and browsing to the "
    //        "required executable (ORA.exe) and selecting the file.  If the executable "
    //        "is valid click the Run button to start the test otherwise check the installation."));
    //        warn = false;
    //    }
    //});

    //connect(ui->openButton, &QPushButton::clicked,
    //        derived.get(), &TonometerManager::select);

    //connect(derived.get(),&TonometerManager::canSelectDatabase,
    //        this,[this](){
    //    foreach(auto button, this->findChildren<QPushButton *>())
    //    {
    //      if("Close" != button->text())
    //        button->setEnabled(false);
    //    }
    //    ui->openButton->setEnabled(true);
    //    static bool warn = true;
    //    if(warn)
    //    {
    //        QMessageBox::warning(
    //        this, QApplication::applicationName(),
    //        tr("Select the database by clicking Open and browsing to the "
    //        "required file (ora.mdb) and selecting the file.  If the database "
    //        "is valid click the Run button to start the test otherwise check the installation."));
    //        warn = false;
    //    }
    //});

    //// Available to start measuring
    ////
    ////connect(derived.get(), &TonometerManager::canMeasure,
    ////        ui->measureWidget, &MeasureWidget::enableMeasure);

    //// Request a measurement from the device
    ////
    ////connect(ui->measureWidget, &MeasureWidget::measure,
    ////    derived.get(), &TonometerManager::measure);

    //// Update the UI with any data
    ////
    ////connect(derived.get(), &TonometerManager::dataChanged,
    ////    ui->measureWidget, &MeasureWidget::updateModelView);

    //// All measurements received: enable write test results
    ////
    ////connect(derived.get(), &TonometerManager::canFinish,
    ////    ui->measureWidget, &MeasureWidget::enableWriteToFile);

    //// Close the application
    ////
    ////connect(ui->measureWidget, &MeasureWidget::closeApplication,
    ////    this, &DialogBase::close);
}
