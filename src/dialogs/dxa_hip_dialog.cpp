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
    ui->setupUi(this);

    //ui->measurementTable->disableMeasureButton();
    //ui->measurementTable->disableFinishButton();
    ui->measurementTable->hideManualEntry();

    this->setWindowTitle("DXA 1");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new DxaHipManager(session));
    QSharedPointer<DxaHipManager> manager = qSharedPointerCast<DxaHipManager>(m_manager);

    ui->testInfoWidget->setSessionInformation(*session);

    QList<TableColumn> columns;
    columns << TableColumn("PATIENT_ID", "Patient ID", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("STUDY_ID", "Study ID", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("MEDIA_STORAGE_UID", "Media UID", new TextDelegate("", QRegExp(), true));

    // device started
    connect(manager.get(), &DxaHipManager::started, ui->measurementTable, [=](QSharedPointer<TestBase> test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager.get(), &DxaHipManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // can finish
    //connect(manager, &DxaHipManager::canFinish, ui->measurementTable, [=]() {
    //    ui->measurementTable->enableFinishButton();
    //});

    // finished
    connect(manager.get(), &DxaHipManager::success, this, &DxaHipDialog::success);

    // critical error
    connect(manager.get(), &DxaHipManager::error, this, &DxaHipDialog::error);

    // data changed
    connect(manager.get(), &DxaHipManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager.get(), &DxaHipManager::measure);

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager.get(), &DxaHipManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager.get(), &DxaHipManager::addManualMeasurement);
}

DxaHipDialog::~DxaHipDialog()
{
    delete ui;
}
