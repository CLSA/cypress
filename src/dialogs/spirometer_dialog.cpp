#include "spirometer_dialog.h"
#include "managers/spirometer/spirometer_manager.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

SpirometerDialog::SpirometerDialog(QWidget *parent, QSharedPointer<SpirometerSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::SpirometerDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new SpirometerManager(session));

    SpirometerManager* manager = static_cast<SpirometerManager*>(m_manager.get());

    ui->testInfoWidget->setSessionInformation(*session);
    ui->measurementTable->disableMeasureButton();

    if (!m_sim) {
        ui->measurementTable->hideMeasureButton();
    }

    QList<TableColumn> columns;

    columns << TableColumn("rank",
                           "Rank of Trial",
                           new NumberDelegate(0, 1000, true, false, false, 2));
    columns << TableColumn("quality_grade",
                           "Acceptability",
                           new ComboBoxDelegate(QStringList({"A", "B", "C", "D", "E", "F"}),
                                                true,
                                                true));
    columns << TableColumn("fvc",
                           "FVC (liters)",
                           new NumberDelegate(0, 1000, true, false, false, 2));
    columns << TableColumn("fev",
                           "FEV (liters)",
                           new NumberDelegate(0, 1000, true, false, false, 2));
    columns << TableColumn("fev1_fvc",
                           "FEV/FVC Ratio (FEV1%)",
                           new NumberDelegate(0, 1000, true, false, false, 2));

    // device started
    connect(manager, &SpirometerManager::started, ui->measurementTable, [=](TestBase *test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager, &SpirometerManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // can finish
    connect(manager, &SpirometerManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager, &SpirometerManager::success, this, &SpirometerDialog::success);

    // critical error
    connect(manager, &SpirometerManager::error, this, &SpirometerDialog::error);

    // data changed
    connect(manager,
            &SpirometerManager::dataChanged,
            ui->measurementTable,
            &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &SpirometerManager::measure);

    connect(ui->measurementTable, &MeasurementTable::enterManualEntry, manager, [=]() {
        manager->setManualEntry(true);
    });

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &SpirometerManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable,
            &MeasurementTable::addMeasurement,
            manager,
            &SpirometerManager::addManualMeasurement);
}

SpirometerDialog::~SpirometerDialog()
{
    delete ui;
}

// set up signal slot connections between GUI front end
// and device management back end
//
void SpirometerDialog::initializeConnections() {}
