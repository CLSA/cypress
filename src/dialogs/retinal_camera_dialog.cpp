#include <QMessageBox>

#include "retinal_camera_dialog.h"

#include "managers/retinal_camera/retinal_camera_manager.h"
#include "ui_retinal_camera_dialog.h"

#include <QCloseEvent>
#include <QMessageBox>

RetinalCameraDialog::RetinalCameraDialog(QWidget *parent,
                                         QSharedPointer<RetinalCameraSession> session)
    : DialogBase{parent, session}
    , ui(new Ui::RetinalCameraDialog)
{
    ui->setupUi(this);

    ui->measurementTable->disableMeasureButton();
    ui->measurementTable->disableFinishButton();

    ui->measurementTable->hideManualEntry();
    ui->measurementTable->hideMeasureButton();

    this->setWindowTitle("Retinal Camera" + (session->getSide() == Side::Left ? QString("(Left)") : QString("(Right)")));
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new RetinalCameraManager(session));

    RetinalCameraManager* manager = static_cast<RetinalCameraManager*>(m_manager.get());

    ui->testInfoWidget->setSessionInformation(*session);

    QList<TableColumn> columns;
    columns << TableColumn("EYE_PICT_VENDOR", "Picture", new TextDelegate("", QRegExp("^1234$"), true));
    columns << TableColumn("EYE_SIDE_VENDOR", "Side", new TextDelegate("", QRegExp(), true));

    // device started
    connect(manager, &RetinalCameraManager::started, ui->measurementTable, [=](TestBase* test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager, &RetinalCameraManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // can finish
    connect(manager, &RetinalCameraManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager, &RetinalCameraManager::success, this, &RetinalCameraDialog::success);

    // critical error
    connect(manager, &RetinalCameraManager::error, this, &RetinalCameraDialog::error);

    // data changed
    connect(manager, &RetinalCameraManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &RetinalCameraManager::measure);

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &RetinalCameraManager::finish);

    // request adding manual measurement
    //connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager, &RetinalCameraManager::addManualMeasurement);

    // update measurement
}

RetinalCameraDialog::~RetinalCameraDialog()
{
    qDebug() << "RetinalCameraDialog::~RetinalCameraDialog";
    delete ui;
}


void RetinalCameraDialog::initializeModel()
{
}

void RetinalCameraDialog::initializeConnections()
{
}
