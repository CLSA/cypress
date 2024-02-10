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

    //ui->measurementTable->disableMeasureButton();
    //ui->measurementTable->disableFinishButton();

    //if (!m_sim)
    //ui->measurementTable->hideMeasureButton();

    this->setWindowTitle("Grip Strength");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new GripStrengthManager(session));

    QSharedPointer<GripStrengthManager> manager = qSharedPointerCast<GripStrengthManager>(m_manager);
    ui->testInfoWidget->setSessionInformation(*session);

    //QList<TableColumn> columns;

    //columns << TableColumn("exam_id", "Exam ID", new TextDelegate());
    //columns << TableColumn("test_id", "Test ID", new TextDelegate());
    //columns << TableColumn("position", "Position", new TextDelegate());
    //columns << TableColumn("trial_side", "Trial Side", new TextDelegate());
    //columns << TableColumn("maximum", "Maximum", new TextDelegate());
    //columns << TableColumn("average", "Average", new TextDelegate());

    //columns << TableColumn("rep_1", "Rep 1", new TextDelegate());
    //columns << TableColumn("rep_2", "Rep 2", new TextDelegate());
    //columns << TableColumn("rep_3", "Rep 3", new TextDelegate());

    //columns << TableColumn("rep_1_exclude", "Rep 1 Exclude", new ComboBoxDelegate(QStringList({"No", "Yes"}), true, true, false));
    //columns << TableColumn("rep_2_exclude", "Rep 2 Exclude", new ComboBoxDelegate(QStringList({"No", "Yes"}), true, true, false));
    //columns << TableColumn("rep_3_exclude", "Rep 3 Exclude", new ComboBoxDelegate(QStringList({"No", "Yes"}), true, true, false));

    // device started
    //connect(manager.get(), &GripStrengthManager::started, ui->measurementTable, [=](QSharedPointer<TestBase> test) {
    //    Q_UNUSED(test)
    //    ui->measurementTable->initializeModel(columns);
    //});

    // can auto measure
    //connect(manager.get(), &GripStrengthManager::canMeasure, ui->measurementTable, [=]() {
    //    ui->measurementTable->enableMeasureButton();
    //});

    //// can finish
    //connect(manager.get(), &GripStrengthManager::canFinish, ui->measurementTable, [=]() {
    //    ui->measurementTable->enableFinishButton();
    //});

    // finished
    connect(manager.get(), &GripStrengthManager::success, this, &GripStrengthDialog::success);

    // critical error
    connect(manager.get(), &GripStrengthManager::error, this, &GripStrengthDialog::error);

    // data changed
    //connect(manager.get(), &GripStrengthManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    //connect(ui->measurementTable, &MeasurementTable::measure, manager.get(), &GripStrengthManager::measure);

    // request finish
    // connect(ui->measurementTable, &MeasurementTable::finish, manager.get(), &GripStrengthManager::finish);

    // request adding manual measurement
    // connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager.get(), &GripStrengthManager::addManualMeasurement);

}

GripStrengthDialog::~GripStrengthDialog()
{
    delete ui;
}
