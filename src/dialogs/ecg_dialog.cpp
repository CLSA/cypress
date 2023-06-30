#include "ecg_dialog.h"
#include "ui_ecg_dialog.h"
#include "managers/ecg/ecg_manager.h"
#include "cypress_application.h"

#include <QCloseEvent>

EcgDialog::EcgDialog(QJsonObject inputData) : ui(new Ui::EcgDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new ECGManager(inputData));
    m_manager->start();
    
    if (Cypress::getInstance().isSimulation()) {
        ui->measureButton->setEnabled(true);
    }

    connect(ui->measureButton, &QPushButton::clicked, m_manager.get(), &ECGManager::measure);
}

void EcgDialog::initializeConnections() {

};

EcgDialog::~EcgDialog()
{
    delete ui;
}

void EcgDialog::userClose()
{
    qDebug() << "ECGDialog::handleClose";
    DialogBase::userClose();
    if (m_user_close)
    {
        m_manager->sendComplete("ecg", m_manager->m_uuid);
    }
    else
    {
        Cypress::getInstance().deviceDialog = nullptr;
        m_manager->sendCancellation("ecg", m_manager->m_uuid);
    }
    
    Cypress::getInstance().forceSessionEnd();
}

void EcgDialog::closeEvent(QCloseEvent* event)
{
    event->accept();
}
