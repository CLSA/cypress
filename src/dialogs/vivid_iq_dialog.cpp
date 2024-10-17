#include "vivid_iq_dialog.h"
#include "ui_vivid_iq_dialog.h"

#include "managers/ultrasound/vivid_iq_manager.h"

VividIQDialog::VividIQDialog(QWidget *parent, QSharedPointer<CypressSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::VividIQDialog)
{
    m_manager.reset(new VividIQManager(session));

    ui->setupUi(this);

    setWindowTitle("Vivid IQ");

    ui->testInfoWidget->setSessionInformation(*session);
    ui->measurementTable->hideManualEntry();
    ui->measurementTable->hideMeasureButton();
    ui->measurementTable->disableFinishButton();

    auto manager = qSharedPointerCast<VividIQManager>(m_manager);

    connect(manager.get(), &VividIQManager::success, this, &VividIQDialog::success);
    connect(manager.get(), &VividIQManager::error, this, &VividIQDialog::error);
    connect(manager.get(), &VividIQManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    QList<TableColumn> columns;
    columns << TableColumn("patient_id", "Patient ID", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("path", "Path", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("size", "Size", new TextDelegate("", QRegExp(), true));

    // device started
    connect(manager.get(), &VividIQManager::started, ui->measurementTable, [=](QSharedPointer<TestBase> test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // data changed
    connect(manager.get(), &VividIQManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager.get(), [=]() {
        ui->measurementTable->disableFinishButton();
        QApplication::processEvents();
        manager->finish();
    });
}

VividIQDialog::~VividIQDialog()
{
    delete ui;
}
