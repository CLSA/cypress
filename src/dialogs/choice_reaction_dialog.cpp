#include "choice_reaction_dialog.h"
#include "managers/choice_reaction/choice_reaction_manager.h"
#include "cypress_application.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

ChoiceReactionDialog::ChoiceReactionDialog(QWidget *parent,
                                           QSharedPointer<ChoiceReactionSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::ChoiceReactionDialog)
{
    ui->setupUi(this);

    ui->measurementTable->removeManualMeasurement();
    ui->measurementTable->disableMeasureButton();
    ui->measurementTable->disableFinishButton();

    this->setWindowTitle("Choice Reaction Test");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new ChoiceReactionManager(session));

    ChoiceReactionManager* manager = static_cast<ChoiceReactionManager*>(m_manager.get());

    ui->testInfoWidget->setSessionInformation(*session);

    QList<TableColumn> columns;

    columns << TableColumn("screen_id", "Screen ID", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("response_correct", "Response Correct", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("elapsed_time", "Elapsed Time", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("correct_position", "Correct Position", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("response_stimulus_interval", "Response Stimulus Interval", new TextDelegate("", QRegExp(), true));

    // device started
    connect(manager, &ChoiceReactionManager::started, ui->measurementTable, [=](TestBase* test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager, &ChoiceReactionManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // auto measure
    connect(manager, &ChoiceReactionManager::measured, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // can finish
    connect(manager, &ChoiceReactionManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager, &ChoiceReactionManager::success, this, &ChoiceReactionDialog::success);

    // critical error
    connect(manager, &ChoiceReactionManager::error, this, &ChoiceReactionDialog::error);

    // data changed
    connect(manager, &ChoiceReactionManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &ChoiceReactionManager::measure);

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &ChoiceReactionManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager, &ChoiceReactionManager::addManualMeasurement);
}

ChoiceReactionDialog::~ChoiceReactionDialog()
{
    delete ui;
}

// set up signal slot connections between GUI front end
// and device management back end
//
void ChoiceReactionDialog::initializeConnections()
{
    //QSharedPointer<ChoiceReactionManager> derived =
   // m_manager.staticCast<ChoiceReactionManager>();

   // if (Cypress::getInstance().isSimulation()) {
   //   //ui->measureWidget->enableMeasure();
   // }
   // else {
   //   // Disable all buttons by default
   //   //
   //   foreach(auto button, this->findChildren<QPushButton *>())
   //   {
   //       if("Close" != button->text())
   //         button->setEnabled(false);

   //       // disable enter key press event passing onto auto focus buttons
   //       //
   //       button->setDefault(false);
   //       button->setAutoDefault(false);
   //   }
   // }


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

  //connect(derived.get(),&ChoiceReactionManager::canSelectRunnable,
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
  //          "required executable (CCB.exe) and selecting the file.  If the executable "
  //          "is valid click the Run button to start the test otherwise check the installation."));
  //          warn = false;
  //      }
  //  });

  //connect(ui->openButton, &QPushButton::clicked,
  //      this, [this,derived]() {
  //          QString fileName =
  //              QFileDialog::getOpenFileName(
  //                  this, tr("Open File"),
  //                  QCoreApplication::applicationDirPath(),
  //                  tr("Applications (*.exe, *)"));

  //          derived->selectRunnable(fileName);
  //      });

  // Available to start measuring
  //
  //connect(derived.get(), &ChoiceReactionManager::canMeasure,
  //        ui->measureWidget, &MeasureWidget::enableMeasure);

  //// Request a measurement from the device
  ////
  //connect(ui->measureWidget, &MeasureWidget::measure,
  //    derived.get(), &ChoiceReactionManager::measure);

  //// Update the UI with any data
  ////
  //connect(derived.get(), &ChoiceReactionManager::dataChanged,
  //    ui->measureWidget, &MeasureWidget::updateModelView);

  //// All measurements received: enable write test results
  ////
  //connect(derived.get(), &ChoiceReactionManager::canFinish,
  //    ui->measureWidget, &MeasureWidget::enableWriteToFile);

  //// Close the application
  ////
  //connect(ui->measureWidget, &MeasureWidget::closeApplication,
  //    this, &DialogBase::close);
}
