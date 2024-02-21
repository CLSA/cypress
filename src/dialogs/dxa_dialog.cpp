#include "dxa_dialog.h"
#include "ui_dxa_dialog.h"

#include "managers/dxa/dxa_manager.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QDesktopServices>
#include <QHostInfo>

DXADialog::DXADialog(QWidget *parent, QSharedPointer<DXASession> session)
    : DialogBase(parent, session)
    , ui(new Ui::DXADialog)
{
    ui->setupUi(this);
    ui->measurementTable->hideManualEntry();

    setWindowTitle("DXA 2");
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new DXAManager(session));
    QSharedPointer<DXAManager> manager = qSharedPointerCast<DXAManager>(m_manager);

    ui->testInfoWidget->setSessionInformation(*session);

    QList<TableColumn> columns;
    columns << TableColumn("PATIENT_ID", "Patient ID", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("STUDY_ID", "Study ID", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("MEDIA_STORAGE_UID", "Media UID", new TextDelegate("", QRegExp(), true));

    // device started
    connect(manager.get(), &DXAManager::started, ui->measurementTable, [=](QSharedPointer<TestBase> test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager.get(), &DXAManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // data changed
    connect(manager.get(), &DXAManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // finished
    connect(manager.get(), &DXAManager::success, this, &DXADialog::success);

    // critical error
    connect(manager.get(), &DXAManager::error, this, &DXADialog::error);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager.get(), &DXAManager::measure);

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager.get(), &DXAManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager.get(), &DXAManager::addManualMeasurement);
}

DXADialog::~DXADialog()
{
    delete ui;
}
