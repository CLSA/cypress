#include "dxa_dialog.h"
#include "ui_dxa_dialog.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDesktopServices>
#include "cypress_application.h"

DXADialog::DXADialog(QWidget* parent, const CypressSession& session) :
    DialogBase(parent, session),
    ui(new Ui::DXADialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    //m_manager.reset(new DXAManager());
    //m_manager->start();

    ui->submitButton->setEnabled(true);
    connect(ui->submitButton, &QPushButton::clicked, m_manager.get(), &DXAManager::measure);
    //connect(m_manager->m_dicomSCP, &DicomSCP::logUpdate, this, &DXADialog::dicomLogUpdate);
    //connect(m_manager->m_dicomSCP, &DicomSCP::dicomFilesReceived, this, &DXADialog::dicomFilesReceived);
}

DXADialog::~DXADialog()
{
    delete ui;
}

void DXADialog::initializeConnections()
{

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

void DXADialog::dicomFilesReceived(const QStringList& dicomFilePaths)
{
    ui->filesList->clear();
    QStringList::ConstIterator iterator;
    for (iterator = dicomFilePaths.begin(); iterator != dicomFilePaths.end(); ++iterator)
    {
       ui->filesList->append((*iterator).toLocal8Bit().constData());
    }

    qDebug() << dicomFilePaths;
    m_manager->dicomFilesReceived(dicomFilePaths);
}

void DXADialog::on_openFileExplorer_released()
{
    QDesktopServices::openUrl(QUrl(QDir::currentPath() + "/dcmtk-3.6.7/storage"));
}


void DXADialog::on_submitButton_clicked()
{
    //m_manager->measure();
}

void DXADialog::userClose()
{
    qDebug() << "DXADialog::handleClose";
    DialogBase::userClose();
    if (m_user_close)
    {
        m_manager->sendComplete("dxa", m_manager->m_uuid);
    }
    else
    {
        //Cypress::getInstance().deviceDialog = nullptr;
        m_manager->sendCancellation("dxa", m_manager->m_uuid);
    }
    //Cypress::getInstance().endSession();
}

void DXADialog::closeEvent(QCloseEvent* event)
{
    event->accept();
}


