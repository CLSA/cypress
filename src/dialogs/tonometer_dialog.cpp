#include "tonometer_dialog.h"
#include "managers/tonometer/tonometer_manager.h"
#include "cypress_application.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

TonometerDialog::TonometerDialog(QWidget *parent, QSharedPointer<TonometerSession> session)
    : DialogBase{parent, session}
    , ui(new Ui::TonometerDialog)
{
    ui->setupUi(this);

    setWindowTitle("Tonometer");

    m_manager.reset(new TonometerManager(session));

    QSharedPointer<TonometerManager> manager = qSharedPointerCast<TonometerManager>(m_manager);

    ui->testInfoWidget->setSessionInformation(*session);

    // finished
    connect(manager.get(), &TonometerManager::success, this, &TonometerDialog::success);

    // critical error
    connect(manager.get(), &TonometerManager::error, this, &TonometerDialog::error);
}

TonometerDialog::~TonometerDialog()
{
    delete ui;
}
