#include "ecg_dialog.h"
#include "ui_ecg_dialog.h"

#include "cypress_application.h"
#include "managers/ecg/ecg_manager.h"

#include <QCloseEvent>

EcgDialog::EcgDialog(QWidget *parent, QSharedPointer<ECGSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::EcgDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new ECGManager(session));

    QSharedPointer<ECGManager> manager = qSharedPointerCast<ECGManager>(m_manager);

    this->setWindowTitle("ECG");
    ui->testInfoWidget->setSessionInformation(*session);

    // finished
    connect(manager.get(), &ECGManager::success, this, &EcgDialog::success);

    // critical error
    connect(manager.get(), &ECGManager::error, this, &EcgDialog::error);
}

EcgDialog::~EcgDialog()
{
    delete ui;
}
