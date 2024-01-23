#include "frax_dialog.h"
#include "managers/frax/frax_manager.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

FraxDialog::FraxDialog(QWidget *parent, QSharedPointer<FraxSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::FraxDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("FRAX");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new FraxManager(session));

    QSharedPointer<FraxManager> manager = qSharedPointerCast<FraxManager>(m_manager);

    // finished
    connect(manager.get(), &FraxManager::success, this, &FraxDialog::success);

    // critical error
    connect(manager.get(), &FraxManager::error, this, &FraxDialog::error);
}

FraxDialog::~FraxDialog()
{
    delete ui;
}
