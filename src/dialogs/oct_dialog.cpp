#include "oct_dialog.h"
#include "ui_oct_dialog.h"

#include "managers/retinal_camera/oct_manager.h"

OCTDialog::OCTDialog(QWidget *parent, QSharedPointer<OCTSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::OCTDialog)
{
    m_manager.reset(new OCTManager(session));

    ui->setupUi(this);

    setWindowTitle("OCT " + (session->getSide() == OCTSession::Left ? QString("(Left)") : QString("(Right)")));

    ui->testInfoWidget->setSessionInformation(*session);
    ui->measurementTable->hideManualEntry();
    ui->measurementTable->disableFinishButton();

    QList<TableColumn> columns;
    columns << TableColumn("name", "Name", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("path", "Path", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("size", "Size", new TextDelegate("", QRegExp(), true));

    auto manager = qSharedPointerCast<OCTManager>(m_manager);
    connect(manager.get(), &OCTManager::started, ui->measurementTable, [=](QSharedPointer<TestBase> test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });
    connect(manager.get(), &OCTManager::canMeasure, this, [=]() {
        ui->measurementTable->enableMeasureButton();
    });
    connect(ui->measurementTable, &MeasurementTable::measure, manager.get(), &OCTManager::measure);
    connect(manager.get(), &OCTManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);
    connect(manager.get(), &OCTManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });
    connect(manager.get(), &OCTManager::cannotFinish, ui->measurementTable, [=]() {
        ui->measurementTable->disableFinishButton();
    });
    connect(ui->measurementTable, &MeasurementTable::finish, manager.get(), [=]() {
       ui->measurementTable->disableFinishButton();
        QApplication::processEvents();
        manager->finish();
    });
    connect(manager.get(), &OCTManager::success, this, &OCTDialog::success);
    connect(manager.get(), &OCTManager::error, this, &OCTDialog::error);
}

OCTDialog::~OCTDialog()
{
    delete ui;
}
