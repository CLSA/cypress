#include "cdtt_dialog.h"
#include "managers/cdtt/cdtt_manager.h"

#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

CDTTDialog::CDTTDialog(QWidget *parent, QSharedPointer<CDTTSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::CDTTDialog)
{
    ui->setupUi(this);

    setWindowTitle("CDTT");
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new CDTTManager(session));

    QSharedPointer<CDTTManager> manager = qSharedPointerCast<CDTTManager>(m_manager);

    ui->testInfoWidget->setSessionInformation(*session);

    // finished
    connect(manager.get(), &CDTTManager::success, this, &CDTTDialog::success);

    // critical error
    connect(manager.get(), &CDTTManager::error, this, &CDTTDialog::error);
}

CDTTDialog::~CDTTDialog()
{
    delete ui;
}
