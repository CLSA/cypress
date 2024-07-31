#include "easyone_dialog.h"
#include "managers/spirometer/easyone_manager.h"

EasyoneDialog::EasyoneDialog(QWidget *parent, QSharedPointer<EasyoneConnectSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::EasyoneDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new EasyoneConnectManager(session));

    auto manager = qSharedPointerCast<EasyoneConnectManager>(m_manager);

    ui->testInfoWidget->setSessionInformation(*session);

    connect(manager.get(), &EasyoneConnectManager::success, this, &EasyoneDialog::success);
    connect(manager.get(), &EasyoneConnectManager::error, this, &EasyoneDialog::error);
}

EasyoneDialog::~EasyoneDialog()
{
    delete ui;
}
