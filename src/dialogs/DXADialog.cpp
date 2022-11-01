#include "DXADialog.h"
#include "ui_dxadialog.h"
#include <QCloseEvent>
#include <QMessageBox>

DXADialog::DXADialog(DXAManager* manager, QWidget *parent) :
    DialogBase(parent), m_manager(manager),
    ui(new Ui::DXADialog)
{
    ui->setupUi(this);
    qDebug() << "manager name: " << m_manager->getName();
    m_manager -> start();

    connect(m_manager->m_dicomSCP, &DicomSCP::logUpdate, this, &DXADialog::dicomLogUpdate);
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

    if(m_verbose)
        qDebug() << "Close event called";

    //QDir dir = QCoreApplication::applicationDirPath();
    //QSettings settings(dir.filePath(m_manager->getGroup() + ".json"), JsonSettings::JsonFormat);

    qDebug() << "sub m_manager finish";
    //m_manager->saveSettings(&settings);
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
    qInfo() << "DXADialog::dicomLogUpdate: " << line;
    ui->textBrowser->append(line);
}



