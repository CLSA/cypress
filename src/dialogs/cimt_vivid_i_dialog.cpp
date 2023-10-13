#include "cimt_vivid_i_dialog.h"
#include "ui_cimt_vivid_i_dialog.h"
#include "managers/ultrasound/vividi_manager.h"
#include "cypress_session.h"

#include <QHostInfo>
#include <QTableWidget>
#include <QHeaderView>


CimtVividiDialog::CimtVividiDialog(QWidget *parent, const CypressSession& session) :
    DialogBase(parent, session),
    ui(new Ui::CimtVividiDialog)
{
    ui->setupUi(this);

    ui->measurementTable->disableMeasureButton();
    ui->measurementTable->disableFinishButton();

    this->setWindowTitle("CIMT");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new VividiManager(session));
    VividiManager* manager = static_cast<VividiManager*>(m_manager.get());

    ui->testInfoWidget->setSessionInformation(session);
    ui->dicomWidget->setDicomLabels("CLSADICOM", QHostInfo::localHostName(), "9001");

    QList<TableColumn> columns;
    columns << TableColumn("STUDY_INSTANCE_ID", "ID", new TextDelegate("", QRegExp("^1234$"), true));
    columns << TableColumn("SIDE", "Side", new ComboBoxDelegate(QStringList() << "LEFT" << "RIGHT", true, false, "Select a side"));

    // device started
    connect(manager, &VividiManager::started, ui->measurementTable, [=](TestBase* test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager, &VividiManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // auto measure
    connect(manager, &VividiManager::measured, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // can finish
    connect(manager, &VividiManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager, &VividiManager::success, this, &CimtVividiDialog::success);

    // critical error
    connect(manager, &VividiManager::error, this, &CimtVividiDialog::error);

    // data changed
    connect(manager, &VividiManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &VividiManager::measure);

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &VividiManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager, &VividiManager::addManualMeasurement);
}

CimtVividiDialog::~CimtVividiDialog()
{
    delete ui;
}
