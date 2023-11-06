#include "dxa_hip_dialog.h"
#include "ui_dxa_hip_dialog.h"

#include "managers/dxa/dxa_hip_manager.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QDesktopServices>
#include <QHostInfo>

DxaHipDialog::DxaHipDialog(QWidget *parent, QSharedPointer<DxaHipSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::DxaHipDialog)
{
    ui->setupUi(this);

    ui->measurementTable->disableMeasureButton();
    ui->measurementTable->disableFinishButton();

    this->setWindowTitle("DXA Hip");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new DxaHipManager(session));
    DxaHipManager* manager = static_cast<DxaHipManager*>(m_manager.get());

    ui->testInfoWidget->setSessionInformation(*session);
    ui->dicomWidget->setDicomLabels("CLSADICOM", QHostInfo::localHostName(), "9001");

    QList<TableColumn> columns;

    // device started
    connect(manager, &DxaHipManager::started, ui->measurementTable, [=](TestBase* test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager, &DxaHipManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // auto measure
    connect(manager, &DxaHipManager::measured, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // can finish
    connect(manager, &DxaHipManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager, &DxaHipManager::success, this, &DxaHipDialog::success);

    // critical error
    connect(manager, &DxaHipManager::error, this, &DxaHipDialog::error);

    // data changed
    connect(manager, &DxaHipManager::dataChanged, ui->measurementTable, &MeasurementTable::handleTestUpdate);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &DxaHipManager::measure);

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &DxaHipManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager, &DxaHipManager::addManualMeasurement);
}

DxaHipDialog::~DxaHipDialog()
{
    delete ui;
}

void DxaHipDialog::dicomServerStarted()
{
    qInfo() << "DxaHipDialog::dicomServerStarted";
}

void DxaHipDialog::dicomServerEnded()
{
    qInfo() << "DxaHipDialog::dicomServerEnded";
}

void DxaHipDialog::dicomLogUpdate(QString line)
{
    //ui->logBrowser->append(line);
}

void DxaHipDialog::dicomFilesReceived(const QStringList& dicomFilePaths)
{
    Q_UNUSED(dicomFilePaths)
    //ui->filesList->clear();
    //QStringList::ConstIterator iterator;
    //for (iterator = dicomFilePaths.begin(); iterator != dicomFilePaths.end(); ++iterator)
    //{
    //   ui->filesList->append((*iterator).toLocal8Bit().constData());
    //}

    //qDebug() << dicomFilePaths;
    //m_manager->dicomFilesReceived(dicomFilePaths);
}

void DxaHipDialog::on_openFileExplorer_released()
{
    QDesktopServices::openUrl(QUrl(QDir::currentPath() + "/dcmtk-3.6.7/storage"));
}


void DxaHipDialog::on_submitButton_clicked()
{
    //m_manager->measure();
}
