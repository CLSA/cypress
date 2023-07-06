#include "ecg_dialog.h"
#include "ui_ecg_dialog.h"
#include "managers/ecg/ecg_manager.h"
#include "cypress_application.h"

#include <QCloseEvent>

EcgDialog::EcgDialog(QWidget* parent, const CypressSession& session)
    : DialogBase(parent, session)
    , ui(new Ui::EcgDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new ECGManager(session));
    m_manager->start();

    if (Cypress::getInstance().isSimulation()) {
        ui->measureButton->setEnabled(true);
    }

    //connect(ui->measureButton, &QPushButton::clicked, m_manager.get(), &ECGManager::measure);
}

EcgDialog::~EcgDialog()
{
    delete ui;
}
