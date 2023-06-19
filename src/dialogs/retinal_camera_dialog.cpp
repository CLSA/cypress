#include <QMessageBox>

#include "cypress_application.h"
#include "retinal_camera_dialog.h"
#include "managers/retinal_camera/retinal_camera_manager.h"
#include "ui_retinal_camera_dialog.h"

#include <QCloseEvent>

RetinalCameraDialog::RetinalCameraDialog(QJsonObject inputData) : ui(new Ui::RetinalCameraDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    if (CypressApplication::getInstance().isSimulation())
    {
        setWindowTitle("Retinal Camera (SIM)");
    }
    else {
        setWindowTitle("Retinal Camera");
    }

    m_manager.reset(new RetinalCameraManager(inputData));
}

RetinalCameraDialog::~RetinalCameraDialog()
{
    qDebug() << "RetinalCameraDialog::~RetinalCameraDialog";
    delete ui;
}

void RetinalCameraDialog::initializeModel()
{
    qDebug() << "RetinalCameraDialog::initializeModel";
}

void RetinalCameraDialog::initializeConnections()
{
    qDebug() << "RetinalCameraDialog::initializeConnections";
    //QMessageBox::critical(this, QApplication::applicationName(), tr("The input does not match the expected barcode for this participant."));

    QSharedPointer<RetinalCameraManager> derived = m_manager.staticCast<RetinalCameraManager>();
    connect(ui->measureButton, &QPushButton::clicked, derived.get(), &RetinalCameraManager::measure);
    connect(ui->completeButton, &QPushButton::clicked, this, &RetinalCameraDialog::userClose);

    QObject::connect(this, &RetinalCameraDialog::accepted, [&]() {
    });

    ui->measureButton->setEnabled(true);
}

void RetinalCameraDialog::userClose()
{
    DialogBase::userClose();
    CypressApplication::getInstance().forceSessionEnd();
}

void RetinalCameraDialog::closeEvent(QCloseEvent* event)
{
    qDebug() << "RetinalCameraDialog::handleClose";
    event->ignore();
    if (m_user_close)
    {
        m_manager->sendComplete("retinal_camera", m_manager->m_uuid);
    }
    else
    {
        CypressApplication::getInstance().dialog = nullptr;
        m_manager->sendCancellation("retinal_camera", m_manager->m_uuid);
    }

    event->accept();
}
