#include "oct_dialog.h"
#include "ui_oct_dialog.h"

#include "managers/oct_manager.h"

OCTDialog::OCTDialog(QWidget *parent, QSharedPointer<OCTSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::OCTDialog)
{
    ui->setupUi(this);
    setWindowTitle("OCT");

    ui->testInfoWidget->setSessionInformation(*session);

    m_manager.reset(new OCTManager(session));
}

OCTDialog::~OCTDialog()
{
    delete ui;
}
