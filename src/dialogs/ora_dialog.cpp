#include "ora_dialog.h"
#include "managers/ora_manager.h"
#include "ui_ora_dialog.h"

ORADialog::ORADialog(QWidget *parent, QSharedPointer<ORASession> session)
    : DialogBase(parent, session)
    , ui(new Ui::ORADialog)
{
    ui->setupUi(this);
    setWindowTitle("Tonometer");

    m_manager.reset(new ORAManager(session));

    QSharedPointer<ORAManager> manager = qSharedPointerCast<ORAManager>(m_manager);

    ui->testInfoWidget->setSessionInformation(*session);

    // finished
    connect(manager.get(), &ORAManager::success, this, &ORADialog::success);

    // critical error
    connect(manager.get(), &ORAManager::error, this, &ORADialog::error);
}

ORADialog::~ORADialog()
{
    delete ui;
}
