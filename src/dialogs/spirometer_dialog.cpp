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

    QSharedPointer<SpirometerManager> manager = qSharedPointerCast<SpirometerManager>(m_manager);

    ui->testInfoWidget->setSessionInformation(*session);
    //ui->measurementTable->disableMeasureButton();

    //QList<TableColumn> columns;
    //columns << TableColumn("rank",
    //                       "Rank of Trial",
    //                       new NumberDelegate(0, 1000, true, false, false, 2));
    //columns << TableColumn("quality_grade",
    //                       "Acceptability",
    //                       new ComboBoxDelegate(QStringList({"A", "B", "C", "D", "E", "F"}),
    //                                            true,
    //                                            true));
    //columns << TableColumn("fvc",
    //                       "FVC (liters)",
    //                       new NumberDelegate(0, 1000, true, false, false, 2));
    //columns << TableColumn("fev",
    //                       "FEV (liters)",
    //                       new NumberDelegate(0, 1000, true, false, false, 2));
    //columns << TableColumn("fev1_fvc",
    //                       "FEV/FVC Ratio (FEV1%)",
    //                       new NumberDelegate(0, 1000, true, false, false, 2));

    // device started
    //connect(manager.get(), &SpirometerManager::started, ui->measurementTable, [=](QSharedPointer<TestBase> test) {
    //    Q_UNUSED(test)
    //    ui->measurementTable->initializeModel(columns);
    //});

    //// can auto measure
    //connect(manager.get(), &SpirometerManager::canMeasure, ui->measurementTable, [=]() {
    //    ui->measurementTable->enableMeasureButton();
    //});

    //// can finish
    //connect(manager.get(), &SpirometerManager::canFinish, ui->measurementTable, [=]() {
    //    ui->measurementTable->enableFinishButton();
    //});

    // finished
    connect(manager.get(), &SpirometerManager::success, this, &SpirometerDialog::success);

    // critical error
    connect(manager.get(), &SpirometerManager::error, this, &SpirometerDialog::error);

    // data changed
    //connect(manager.get(),
    //        &SpirometerManager::dataChanged,
    //        ui->measurementTable,
    //        &MeasurementTable::handleTestUpdate);

    //// request auto measure
    //connect(ui->measurementTable, &MeasurementTable::measure, manager.get(), &SpirometerManager::measure);

    //connect(ui->measurementTable, &MeasurementTable::enterManualEntry, manager.get(), [=]() {
    //    manager->setManualEntry(true);
    //});

    //// request finish
    //connect(ui->measurementTable, &MeasurementTable::finish, manager.get(), &SpirometerManager::finish);

    //// request adding manual measurement
    //connect(ui->measurementTable,
    //        &MeasurementTable::addMeasurement,
    //        manager.get(),
    //        &SpirometerManager::addManualMeasurement);
}

SpirometerDialog::~SpirometerDialog()
{
    delete ui;
}
