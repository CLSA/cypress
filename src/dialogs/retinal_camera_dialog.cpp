#include <QMessageBox>

#include "cypress_application.h"
#include "retinal_camera_dialog.h"
#include "managers/retinal_camera/retinal_camera_manager.h"
#include "ui_retinal_camera_dialog.h"

#include <QCloseEvent>

RetinalCameraDialog::RetinalCameraDialog(QWidget* parent, const CypressSession& session) :
    DialogBase { parent, session },
    ui(new Ui::RetinalCameraDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    if (Cypress::getInstance().isSimulation())
    {
        setWindowTitle("Retinal Camera (SIM)");
    }
    else {
        setWindowTitle("Retinal Camera");
    }

    //m_manager.reset(new RetinalCameraManager());
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
    //connect(ui->completeButton, &QPushButton::clicked, this, &RetinalCameraDialog::userClose);

    QObject::connect(this, &RetinalCameraDialog::accepted, [&]() {
    });

    ui->measureButton->setEnabled(true);
}
