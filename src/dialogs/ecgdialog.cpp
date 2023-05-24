#include "ecgdialog.h"
#include "ui_ecgdialog.h"
#include "managers/ecg/ECGManager.h"
#include "CypressApplication.h"

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
