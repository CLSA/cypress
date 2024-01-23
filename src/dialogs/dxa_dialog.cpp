#include "dxa_dialog.h"
#include "ui_dxa_dialog.h"

#include "managers/dxa/dxa_manager.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QDesktopServices>
#include <QHostInfo>

DXADialog::DXADialog(QWidget *parent, QSharedPointer<DXASession> session)
    : DialogBase(parent, session)
    , ui(new Ui::DXADialog)
{
    ui->setupUi(this);

    ui->measurementTable->disableMeasureButton();
    //ui->measurementTable->disableFinishButton();
    ui->measurementTable->hideManualEntry();
    ui->measurementTable->hideMeasureButton();

    this->setWindowTitle("DXA 2");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new DXAManager(session));
    QSharedPointer<DXAManager> manager = qSharedPointerCast<DXAManager>(m_manager);

    ui->testInfoWidget->setSessionInformation(*session);

    QList<TableColumn> columns;
    columns << TableColumn("patient_id", "Patient ID", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("study_id", "Study ID", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("media_storage_uid", "Media UID", new TextDelegate("", QRegExp(), true));

    // finished
    connect(manager.get(), &DXAManager::success, this, &DXADialog::success);

    // critical error
    connect(manager.get(), &DXAManager::error, this, &DXADialog::error);
}

DXADialog::~DXADialog()
{
    delete ui;
}
