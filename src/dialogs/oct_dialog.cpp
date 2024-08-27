#include "oct_dialog.h"
#include "ui_oct_dialog.h"

#include "managers/retinal_camera/oct_manager.h"

OCTDialog::OCTDialog(QWidget *parent, QSharedPointer<OCTSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::OCTDialog)
{
    ui->setupUi(this);
    setWindowTitle("OCT " + (session->getSide() == OCTSession::Left ? QString("(Left)") : QString("(Right)")));

    ui->testInfoWidget->setSessionInformation(*session);
    m_manager.reset(new OCTManager(session));

    auto manager = qSharedPointerCast<OCTManager>(m_manager);
    connect(manager.get(), &OCTManager::success, this, &OCTDialog::success);
    connect(manager.get(), &OCTManager::error, this, &OCTDialog::error);
}

OCTDialog::~OCTDialog()
{
    delete ui;
}
