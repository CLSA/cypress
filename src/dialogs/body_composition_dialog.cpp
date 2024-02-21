#include "body_composition_dialog.h"
#include "managers/body_composition/body_composition_manager.h"
#include "cypress_application.h"

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

BodyCompositionDialog::BodyCompositionDialog(QWidget *parent, QSharedPointer<CypressSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::BodyCompositionDialog)
{
    ui->setupUi(this);
    m_manager.reset(new BodyCompositionManager(session));
}

BodyCompositionDialog::~BodyCompositionDialog()
{
    delete ui;
}
