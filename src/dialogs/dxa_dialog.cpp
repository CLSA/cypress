#include "dxa_dialog.h"
#include "ui_dxa_dialog.h"

#include "managers/dxa/dxa_manager.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QDesktopServices>
#include <QHostInfo>

DXADialog::DXADialog(QWidget *parent, QSharedPointer<DXASession> session)
    : DialogBase(parent, session)
    , ui(new Ui::DXADialog)
{
    ui->setupUi(this);

    ui->measurementTable->disableMeasureButton();
    ui->measurementTable->disableFinishButton();
    ui->measurementTable->hideManualEntry();
    ui->measurementTable->hideMeasureButton();

    this->setWindowTitle("DXA 2");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new DXAManager(session));
    DXAManager* manager = static_cast<DXAManager*>(m_manager.get());

    ui->testInfoWidget->setSessionInformation(*session);

    ui->dicomWidget->setDicomLabels("ONYXDICOM", QHostInfo::localHostName(), "8900");
    ui->dicomWidget->setReadOnly(true);

    QList<TableColumn> columns;

    // device started
    connect(manager, &DXAManager::started, ui->measurementTable, [=](TestBase* test) {
        Q_UNUSED(test)
        ui->measurementTable->initializeModel(columns);
    });

    // can auto measure
    connect(manager, &DXAManager::canMeasure, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
    });

    // auto measure
    connect(manager,
            &DXAManager::updateDicomTable,
            ui->measurementTable,
            &MeasurementTable::handleDicomFiles);

    // can finish
    connect(manager, &DXAManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    // finished
    connect(manager, &DXAManager::success, this, &DXADialog::success);

    // critical error
    connect(manager, &DXAManager::error, this, &DXADialog::error);

    // request auto measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager, &DXAManager::measure);

    // request finish
    connect(ui->measurementTable, &MeasurementTable::finish, manager, &DXAManager::finish);

    // request adding manual measurement
    connect(ui->measurementTable, &MeasurementTable::addMeasurement, manager, &DXAManager::addManualMeasurement);
}

DXADialog::~DXADialog()
{
    delete ui;
}

void DXADialog::dicomServerStarted()
{
    qInfo() << "DXADialog::dicomServerStarted";
}

void DXADialog::dicomServerEnded()
{
    qInfo() << "DXADialog::dicomServerEnded";
}

void DXADialog::dicomLogUpdate(QString line)
{
    //ui->logBrowser->append(line);
}

void DXADialog::dicomFilesReceived(const QStringList& dicomFilePaths)
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
