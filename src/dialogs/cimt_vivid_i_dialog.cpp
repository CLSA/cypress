#include "cimt_vivid_i_dialog.h"
#include "ui_cimt_vivid_i_dialog.h"

#include "managers/ultrasound/vividi_manager.h"

#include <QHostInfo>
#include <QTableWidget>
#include <QHeaderView>

CimtVividiDialog::CimtVividiDialog(QWidget *parent, QSharedPointer<UltrasoundSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::CimtVividiDialog)
{
    ui->setupUi(this);

    ui->measurementTable->disableMeasureButton();
    //ui->measurementTable->disableFinishButton();
    ui->measurementTable->hideMeasureButton();
    ui->measurementTable->hideManualEntry();
    ui->measurementTable->setTitle("Files Received");

    this->setWindowTitle("CIMT");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new VividiManager(session));
    VividiManager* manager = static_cast<VividiManager*>(m_manager.get());

    ui->testInfoWidget->setSessionInformation(*session);

    QList<TableColumn> columns;

    columns << TableColumn("patient_id", "Patient ID", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("study_id", "Study ID", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("name", "Name", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("side",
                           "Side",
                           new ComboBoxDelegate(QStringList() << "Left"
                                                              << "Right",
                                                true,
                                                false,
                                                "Select a side"));

    // device/server started
    connect(manager, &VividiManager::started, ui->measurementTable, [=](QSharedPointer<TestBase> test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // setup complete, can now measure
    connect(manager, &VividiManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &VividiManager::measure);

    // measures valid, can finish test
    connect(manager, &VividiManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &VividiManager::finish);

    // successful test
    connect(manager, &VividiManager::success, this, &CimtVividiDialog::success);

    // critical error
    connect(manager, &VividiManager::error, this, &CimtVividiDialog::error);

    // data changed
    connect(manager, &VividiManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager, &VividiManager::addManualMeasurement);
}

CimtVividiDialog::~CimtVividiDialog()
{
    delete ui;
}
