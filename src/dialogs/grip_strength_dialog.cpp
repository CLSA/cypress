#include "grip_strength_dialog.h"
#include "managers/grip_strength/grip_strength_manager.h"


#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>


GripStrengthDialog::GripStrengthDialog(QWidget* parent, const CypressSession& session):
    DialogBase(parent, session),
    ui(new Ui::GripStrengthDialog)
{
    ui->setupUi(this);
    ui->measurementTable->disableMeasureButton();
    ui->measurementTable->disableFinishButton();

    this->setWindowTitle("Grip Strength");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new GripStrengthManager(session));

    GripStrengthManager* manager = static_cast<GripStrengthManager*>(m_manager.get());
    ui->testInfoWidget->setSessionInformation(session);

    QList<TableColumn> columns;

    columns << TableColumn("EXAM_ID", "Exam ID", new TextDelegate());
    columns << TableColumn("TEST_ID", "Test ID", new TextDelegate());
    columns << TableColumn("POSITION", "Position", new TextDelegate());
    columns << TableColumn("TRIAL_SIDE", "Trial Side", new TextDelegate());
    columns << TableColumn("MAXIMUM", "Maximum", new TextDelegate());
    columns << TableColumn("AVERAGE", "Average", new TextDelegate());

    columns << TableColumn("REP_1", "Rep 1", new TextDelegate());
    columns << TableColumn("REP_2", "Rep 2", new TextDelegate());
    columns << TableColumn("REP_3", "Rep 3", new TextDelegate());

    columns << TableColumn("REP_1_Exclude", "Rep 1 Exclude", new TextDelegate());
    columns << TableColumn("REP_2_Exclude", "Rep 2 Exclude", new TextDelegate());
    columns << TableColumn("REP_3_Exclude", "Rep 3 Exclude", new TextDelegate());

    // device started
    connect(manager, &GripStrengthManager::started, ui->measurementTable, [=](TestBase<Measurement>* test) {
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager, &GripStrengthManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // auto measure
    connect(manager, &GripStrengthManager::measured, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // can finish
    connect(manager, &GripStrengthManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager, &GripStrengthManager::success, this, &GripStrengthDialog::success);

    // critical error
    connect(manager, &GripStrengthManager::error, this, &GripStrengthDialog::error);

    // data changed
    connect(manager, &GripStrengthManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &GripStrengthManager::measure);

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &GripStrengthManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager, &GripStrengthManager::addManualMeasurement);

}

GripStrengthDialog::~GripStrengthDialog()
{
    delete ui;
}

void GripStrengthDialog::initializeConnections()
{
    //m_manager.start();
}

bool GripStrengthDialog::handleClose() {
    return this -> close();
}

void GripStrengthDialog::enableMeasure() {
    //ui->measureWidget->enableMeasure();
}

void GripStrengthDialog::initializeModel()
{
}
