#include "hearcon_dialog.h"
#include "ui_hearcon_dialog.h"

#include "managers/audiometer/hearcon_manager.h"

HearconDialog::HearconDialog(QWidget *parent, QSharedPointer<HearconSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::HearconDialog)
{
    ui->setupUi(this);
    ui->measurementTable->disableFinishButton();
    ui->testInfoWidget->setSessionInformation(*session, "RA660");

    m_manager.reset(new HearconManager(session));
    auto manager = qSharedPointerCast<HearconManager>(m_manager);

    QList<TableColumn> columns;
    columns << TableColumn("side", "Side", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("test", "Test", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("level", "Level", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("pass", "Pass", new TextDelegate("", QRegExp(), true));

    // device started
    connect(manager.get(), &HearconManager::started, ui->measurementTable, [=](QSharedPointer<TestBase> test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager.get(), &HearconManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });
    connect(manager.get(), &HearconManager::cannotMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->disableMeasureButton();
    });

    // can finish
    connect(manager.get(), &HearconManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });
    connect(manager.get(), &HearconManager::cannotFinish, ui->measurementTable, [=]() {
        ui->measurementTable->disableFinishButton();
    });

    // finished
    connect(manager.get(), &HearconManager::success, this, &HearconDialog::success);

    // critical error
    connect(manager.get(), &HearconManager::error, this, &HearconDialog::error);

    // data changed
    connect(manager.get(), &HearconManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager.get(), &HearconManager::measure);

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager.get(), &HearconManager::finish);
}

HearconDialog::~HearconDialog()
{
    delete ui;
}
