#include "cimt_vivid_i_dialog.h"
#include "ui_cimt_vivid_i_dialog.h"
#include "managers/ultrasound/vividi_manager.h"
#include "cypress_session.h"

CimtVividiDialog::CimtVividiDialog(const CypressSession& session, QWidget *parent) :
    DialogBase(parent, session),
    ui(new Ui::CimtVividiDialog)
{
    ui->setupUi(this);

    m_manager.reset(new VividiManager(session));
}

CimtVividiDialog::~CimtVividiDialog()
{
    delete ui;
}
