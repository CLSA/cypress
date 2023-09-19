#include <QMessageBox>

#include "retinal_camera_dialog.h"
#include "managers/retinal_camera/retinal_camera_manager.h"
#include "ui_retinal_camera_dialog.h"

#include <QCloseEvent>
#include <QMessageBox>


RetinalCameraDialog::RetinalCameraDialog(QWidget* parent, const CypressSession& session) :
    DialogBase { parent, session },
    ui(new Ui::RetinalCameraDialog)
{
    ui->setupUi(this);
    ui->measurementTable->disableMeasureButton();
    ui->measurementTable->disableFinishButton();

    this->setWindowTitle("Retinal Camera");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new RetinalCameraManager(session));

    RetinalCameraManager* manager = static_cast<RetinalCameraManager*>(m_manager.get());

    QList<TableColumn> columns;

    columns << TableColumn("EYE_PICT_VENDOR", "Picture", new TextDelegate("", QRegExp("^1234$"), true));
    columns << TableColumn("EYE_SIDE_VENDOR", "Side", new ComboBoxDelegate(QStringList() << "LEFT" << "RIGHT", true, false, "Select a side"));

    connect(manager, &RetinalCameraManager::started, ui->measurementTable, [=](TestBase<Measurement>* test) {
        ui->measurementTable->initializeModel(columns);
    });

    // Can measure
    connect(manager, &RetinalCameraManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // Request measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &RetinalCameraManager::measure);

    // Measured
    connect(manager, &RetinalCameraManager::measured, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // Can finish (valid)
    connect(manager, &RetinalCameraManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // Request Finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &RetinalCameraManager::finish);

    // Finished
    connect(manager, &RetinalCameraManager::success, this, &RetinalCameraDialog::success);

    // Critical Error
    connect(manager, &RetinalCameraManager::error, this, &RetinalCameraDialog::error);
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
