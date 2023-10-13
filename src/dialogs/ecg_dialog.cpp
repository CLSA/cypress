#include "ecg_dialog.h"
#include "ui_ecg_dialog.h"
#include "managers/ecg/ecg_manager.h"
#include "cypress_application.h"

#include <QCloseEvent>

EcgDialog::EcgDialog(QWidget* parent, const CypressSession& session)
    : DialogBase(parent, session)
    , ui(new Ui::EcgDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new ECGManager(session));

    ECGManager* manager = static_cast<ECGManager*>(m_manager.get());

    ui->testInfoWidget->setSessionInformation(session);

    QList<TableColumn> columns;

    columns << TableColumn("ventricular_rate", "Ventricular Rate", new TextDelegate());
    columns << TableColumn("pq_interval", "PQ Interval", new TextDelegate());
    columns << TableColumn("p_duration", "P Duration", new TextDelegate());
    columns << TableColumn("qrs_duration", "QRS Duration", new TextDelegate());
    columns << TableColumn("qt_interval", "QT Interval", new TextDelegate());
    columns << TableColumn("qtc_interval", "QTC interval", new TextDelegate());
    columns << TableColumn("rr_interval", "RR interval", new TextDelegate());
    columns << TableColumn("pp_interval", "PP invterval", new TextDelegate());
    columns << TableColumn("p_axis", "P Axis", new TextDelegate());
    columns << TableColumn("r_axis", "R Axis", new TextDelegate());
    columns << TableColumn("t_axis", "T Axis", new TextDelegate());
    columns << TableColumn("qrs_number", "QRS number", new TextDelegate());
    columns << TableColumn("p_onset", "P Onset", new TextDelegate());
    columns << TableColumn("p_offset", "P Offset", new TextDelegate());
    columns << TableColumn("q_onset", "Q Onset", new TextDelegate());
    columns << TableColumn("q_offset", "Q Offset", new TextDelegate());
    columns << TableColumn("t_offset", "T Offset", new TextDelegate());

    // device started
    connect(manager, &ECGManager::started, ui->measurementTable, [=](TestBase* test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager, &ECGManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // auto measure
    connect(manager, &ECGManager::measured, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // can finish
    connect(manager, &ECGManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager, &ECGManager::success, this, &EcgDialog::success);

    // critical error
    connect(manager, &ECGManager::error, this, &EcgDialog::error);

    // data changed
    connect(manager, &ECGManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &ECGManager::measure);

    connect(ui->measurementTable, &MeasurementTable::enterManualEntry, manager, [=]() {
        manager->setManualEntry(true);
    });

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &ECGManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager, &ECGManager::addManualMeasurement);

}

EcgDialog::~EcgDialog()
{
    delete ui;
}
