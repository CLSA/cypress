#include "DXADialog.h"
#include "ui_dxadialog.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDesktopServices>

DXADialog::DXADialog(QString uuid) : ui(new Ui::DXADialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_uuid = uuid;
    m_manager.reset(new DXAManager());
    m_manager -> start();

    connect(m_manager->m_dicomSCP, &DicomSCP::logUpdate, this, &DXADialog::dicomLogUpdate);
    connect(m_manager->m_dicomSCP, &DicomSCP::dicomFilesReceived, this, &DXADialog::dicomFilesReceived);
}

DXADialog::~DXADialog()
{
    delete ui;
}

void DXADialog::initializeConnections()
{

}

void DXADialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
    if (QMessageBox::No == QMessageBox::question(this, "Close Confirmation", "Are you sure you want to exit? Any unsaved changes will be lost.", QMessageBox::Yes | QMessageBox::No)) {
        return;
    }

    m_manager->finish();
    event->accept();
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
    ui->logBrowser->append(line);
}

void DXADialog::dicomFilesReceived(QStringList& dicomFilePaths)
{
    ui->filesList->clear();
    QStringList::Iterator iterator;
    for (iterator = dicomFilePaths.begin(); iterator != dicomFilePaths.end(); ++iterator)
    {
       ui->filesList->append((*iterator).toLocal8Bit().constData());
    }

    qDebug() << dicomFilePaths;

    m_manager->dicomFilesReceived(dicomFilePaths);
}

void DXADialog::on_openFileExplorer_released()
{
    QDesktopServices::openUrl(QUrl("C:/work/clsa/cypress/dcmtk-3.6.7/storage"));
}

