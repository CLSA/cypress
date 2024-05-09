#include "retinal_camera_dialog.h"

#include "managers/retinal_camera/retinal_camera_manager.h"
#include "ui_retinal_camera_dialog.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QMessageBox>

RetinalCameraDialog::RetinalCameraDialog(QWidget *parent,
                                         QSharedPointer<RetinalCameraSession> session)
    : DialogBase{parent, session}
    , ui(new Ui::RetinalCameraDialog)
{
    ui->setupUi(this);
    setWindowTitle("Retinal Camera " + (session->getSide() == Side::Left ? QString("(Left)") : QString("(Right)")));

    ui->testInfoWidget->setSessionInformation(*session);
    m_manager.reset(new RetinalCameraManager(session));

    auto manager = qSharedPointerCast<RetinalCameraManager>(m_manager);
    connect(manager.get(), &RetinalCameraManager::success, this, &RetinalCameraDialog::success);
    connect(manager.get(), &RetinalCameraManager::error, this, &RetinalCameraDialog::error);
}

RetinalCameraDialog::~RetinalCameraDialog()
{
    delete ui;
}
