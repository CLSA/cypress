#include "general_proxy_form_dialog.h"
#include "managers/general_proxy_form/general_proxy_manager.h"

#include "ui_general_proxy_form_dialog.h"



GeneralProxyFormDialog::GeneralProxyFormDialog(QWidget *parent, QSharedPointer<GenProxySession> session)
    : DialogBase(parent, session)
    , ui(new Ui::GeneralProxyFormDialog)
{
    ui->setupUi(this);

    setWindowTitle("General Proxy Consent Form");
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new GeneralProxyManager(session));

    auto manager = qSharedPointerCast<GeneralProxyManager>(m_manager);

    connect(manager.get(), &GeneralProxyManager::success, this, &GeneralProxyFormDialog::success);
    connect(manager.get(), &GeneralProxyManager::error, this, &GeneralProxyFormDialog::error);
}

GeneralProxyFormDialog::~GeneralProxyFormDialog()
{
    delete ui;
}
