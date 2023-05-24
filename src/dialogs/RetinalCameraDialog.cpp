#include <QMessageBox>

#include "CypressApplication.h"
#include "RetinalCameraDialog.h"
#include "managers/retinal_camera/RetinalCameraManager.h"
#include "ui_RetinalCameraDialog.h"

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
    this->run();
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
    QSharedPointer<RetinalCameraManager> derived = m_manager.staticCast<RetinalCameraManager>();
    connect(ui->measureButton, &QPushButton::clicked, derived.get(), &RetinalCameraManager::measure);
    //QMessageBox::critical(this, QApplication::applicationName(), tr("The input does not match the expected barcode for this participant."));

    ui->measureButton->setEnabled(true);
}

bool RetinalCameraDialog::handleClose()
{
    qDebug() << "RetinalCameraDialog::handleClose";
    return this -> close();
}
