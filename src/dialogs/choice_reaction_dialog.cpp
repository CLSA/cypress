#include "choice_reaction_dialog.h"
#include "managers/choice_reaction/choice_reaction_manager.h"
#include "cypress_application.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

ChoiceReactionDialog::ChoiceReactionDialog(QWidget *parent,
                                           QSharedPointer<ChoiceReactionSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::ChoiceReactionDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Choice Reaction Test");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    QSharedPointer<ChoiceReactionManager> manager(new ChoiceReactionManager(session));
    m_manager = manager;

    ui->testInfoWidget->setSessionInformation(*session);

    // finished
    connect(manager.get(), &ChoiceReactionManager::success, this, &ChoiceReactionDialog::success);

    // critical error
    connect(manager.get(), &ChoiceReactionManager::error, this, &ChoiceReactionDialog::error);
}

ChoiceReactionDialog::~ChoiceReactionDialog()
{
    delete ui;
}
