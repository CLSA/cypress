#include "dxa_hip_dialog.h"
#include "ui_dxa_hip_dialog.h"

#include "managers/dxa/dxa_hip_manager.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QDesktopServices>
#include <QHostInfo>

DxaHipDialog::DxaHipDialog(QWidget *parent, QSharedPointer<DxaHipSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::DxaHipDialog)
{
    auto manager = qSharedPointerCast<DxaHipManager>(m_manager);
    m_manager.reset(new DxaHipManager(session));

    ui->setupUi(this);
    setWindowTitle("DXA 1");

    ui->testInfoWidget->setSessionInformation(*session);
    ui->measurementTable->hideManualEntry();

    ui->measurementTable->disableMeasureButton();
    ui->measurementTable->disableFinishButton();

    initManager();
    initMeasurementTable();
}

void DxaHipDialog::initManager() {
    auto manager = qSharedPointerCast<DxaHipManager>(m_manager);

    // finished
    connect(manager.get(), &DxaHipManager::success, this, &DxaHipDialog::success);

    // critical error
    connect(manager.get(), &DxaHipManager::error, this, &DxaHipDialog::error);

    // can auto measure
    connect(manager.get(), &DxaHipManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    //
    connect(ui->measurementTable, &MeasurementTable::measure, manager.get(), [=]() {
        ui->measurementTable->disableMeasureButton();
        QApplication::processEvents();
        manager->measure();
    });

    connect(manager.get(), &DxaHipManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->disableMeasureButton();
        ui->measurementTable->enableFinishButton();
    });
}

void DxaHipDialog::initMeasurementTable() {
    auto manager = qSharedPointerCast<DxaHipManager>(m_manager);

    QList<TableColumn> columns;
    columns << TableColumn("PATIENT_ID", "Patient ID", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("STUDY_ID", "Study ID", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("MEDIA_STORAGE_UID", "Media UID", new TextDelegate("", QRegExp(), true));

    // device started
    connect(manager.get(), &DxaHipManager::started, ui->measurementTable, [=](QSharedPointer<TestBase> test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager.get(), &DxaHipManager::addManualMeasurement);

    // data changed
    connect(manager.get(), &DxaHipManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager.get(), &DxaHipManager::finish);

    connect(manager.get(), &DxaHipManager::status, ui->testInfoWidget, &TestInfoWidget::setStatus);
}

DxaHipDialog::~DxaHipDialog()
{
    delete ui;
}
