#include "watch_bp_dialog.h"
#include "managers/watch_bp_manager.h"
#include "ui_watch_bp_dialog.h"

WatchBPDialog::WatchBPDialog(QWidget *parent, QSharedPointer<WatchBPSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::WatchBPDialog)
{
    ui->setupUi(this);

    m_manager.reset(new WatchBPManager(session));
    auto manager = qSharedPointerCast<WatchBPManager>(m_manager);

    ui->testInfoWidget->setSessionInformation(*session);

    ui->measurementTable->hideMeasureButton();
    ui->measurementTable->enableRemoval(true);

    QList<TableColumn> columns;
    columns << TableColumn("reading_number",
                           "#",
                           new NumberDelegate(0, 1000, true, false, false, 2));
    columns << TableColumn("systolic", "Systolic (mmHg)", new NumberDelegate(0, 200, false));
    columns << TableColumn("diastolic", "Diastolic (mmHg)", new NumberDelegate(0, 200, false));
    columns << TableColumn("pulse", "Pulse (bpm)", new NumberDelegate(0, 200, false));
    columns << TableColumn("start_time", "Start time", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("end_time", "End time", new TextDelegate("", QRegExp(), true));

    connect(manager.get(), &WatchBPManager::started, ui->measurementTable, [=](QSharedPointer<TestBase> test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    connect(manager.get(), &WatchBPManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });
    connect(manager.get(), &WatchBPManager::cannotFinish, ui->measurementTable, [=]() {
        ui->measurementTable->disableFinishButton();
    });

    connect(manager.get(), &WatchBPManager::success, this, &WatchBPDialog::success);
    connect(manager.get(), &WatchBPManager::error, this, &WatchBPDialog::error);
    connect(manager.get(), &WatchBPManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    connect(ui->measurementTable, &MeasurementTable::enterManualEntry, manager.get(), [=]() {

    });

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager.get(), &WatchBPManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager.get(), &WatchBPManager::addManualMeasurement);
    //connect(ui->measurementTable, &MeasurementTable::removeMeasurement, manager.get(), &WatchBPManager::removeMeasurement);
}

WatchBPDialog::~WatchBPDialog()
{
    delete ui;
}
