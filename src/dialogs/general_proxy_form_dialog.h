#ifndef GENERAL_PROXY_FORM_DIALOG_H
#define GENERAL_PROXY_FORM_DIALOG_H

#include <QWidget>
#include "server/sessions/gen_proxy_session.h"

namespace Ui {
class GeneralProxyFormDialog;
}

class GeneralProxyFormDialog : public DialogBase
{
    Q_OBJECT

public:
    explicit GeneralProxyFormDialog(QWidget *parent, QSharedPointer<GenProxySession> session);
    ~GeneralProxyFormDialog();

private:
    Ui::GeneralProxyFormDialog *ui;
};

#endif // GENERAL_PROXY_FORM_DIALOG_H
