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

    if (CypressApplication::getInstance().isSimulation()) {
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
    DialogBase::userClose();
    CypressApplication::getInstance().forceSessionEnd();
}

void EcgDialog::closeEvent(QCloseEvent* event)
{
    qDebug() << "ECGDialog::handleClose";
    event->ignore();
    if (m_user_close)
    {
        m_manager->sendComplete("ecg", m_manager->m_uuid);
    }
    else
    {
        CypressApplication::getInstance().dialog = nullptr;
        m_manager->sendCancellation("ecg", m_manager->m_uuid);
    }
    event->accept();
}
