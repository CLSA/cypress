#include "grip_strength_dialog.h"
#include "managers/grip_strength/grip_strength_manager.h"

#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>

GripStrengthDialog::GripStrengthDialog(QWidget *parent, QSharedPointer<GripStrengthSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::GripStrengthDialog)
{
    ui->setupUi(this);

    ui->measurementTable->disableMeasureButton();
    //ui->measurementTable->disableFinishButton();

    if (!m_sim)
        ui->measurementTable->hideMeasureButton();

    this->setWindowTitle("Grip Strength");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new GripStrengthManager(session));

    QSharedPointer<GripStrengthManager> manager = qSharedPointerCast<GripStrengthManager>(m_manager);
    ui->testInfoWidget->setSessionInformation(*session);

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

    columns << TableColumn("REP_1_Exclude", "Rep 1 Exclude", new ComboBoxDelegate(QStringList({"No", "Yes"}), true, true, false));
    columns << TableColumn("REP_2_Exclude", "Rep 2 Exclude", new ComboBoxDelegate(QStringList({"No", "Yes"}), true, true, false));
    columns << TableColumn("REP_3_Exclude", "Rep 3 Exclude", new ComboBoxDelegate(QStringList({"No", "Yes"}), true, true, false));

    // device started
    connect(manager.get(), &GripStrengthManager::started, ui->measurementTable, [=](QSharedPointer<TestBase> test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager.get(), &GripStrengthManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // can finish
    connect(manager.get(), &GripStrengthManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager.get(), &GripStrengthManager::success, this, &GripStrengthDialog::success);

    // critical error
    connect(manager.get(), &GripStrengthManager::error, this, &GripStrengthDialog::error);

    // data changed
    connect(manager.get(), &GripStrengthManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager.get(), &GripStrengthManager::measure);

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager.get(), &GripStrengthManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager.get(), &GripStrengthManager::addManualMeasurement);

}

GripStrengthDialog::~GripStrengthDialog()
{
    delete ui;
}
