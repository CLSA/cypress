#include "audiometer_dialog.h"
#include "managers/audiometer/audiometer_manager.h"

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

AudiometerDialog::AudiometerDialog(QWidget *parent, QSharedPointer<AudiometerSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::AudiometerDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);
    m_manager.reset(new AudiometerManager(session));

    QSharedPointer<AudiometerManager> manager = qSharedPointerCast<AudiometerManager>(m_manager);

    ui->testInfoWidget->setSessionInformation(*session);

    QList<TableColumn> columns;
    columns << TableColumn("side", "Side", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("test", "Test", new TextDelegate("", QRegExp(), true));
    columns << TableColumn("level", "Level", new TextDelegate("", QRegExp(), true));

    // device started
    connect(manager.get(), &AudiometerManager::started, ui->measurementTable, [=](QSharedPointer<TestBase> test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager.get(), &AudiometerManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // can finish
    connect(manager.get(), &AudiometerManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager.get(), &AudiometerManager::success, this, &AudiometerDialog::success);

    // critical error
    connect(manager.get(), &AudiometerManager::error, this, &AudiometerDialog::error);

    // data changed
    connect(manager.get(), &AudiometerManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager.get(), &AudiometerManager::measure);

    connect(ui->measurementTable, &MeasurementTable::enterManualEntry, manager.get(), [=]() {
        manager->setManualEntry(true);
    });

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager.get(), &AudiometerManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager.get(), &AudiometerManager::addManualMeasurement);
}

AudiometerDialog::~AudiometerDialog()
{
    delete ui;
}
