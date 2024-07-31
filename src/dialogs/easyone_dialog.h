#ifndef EASYONE_DIALOG_H
#define EASYONE_DIALOG_H

#include "dialog_base.h"
#include "ui_easyone_dialog.h"

#include "server/sessions/easyone_connect_session.h"

class EasyoneDialog : public DialogBase, public Ui::EasyoneDialog
{
    Q_OBJECT

public:
    explicit EasyoneDialog(QWidget *parent, QSharedPointer<EasyoneConnectSession> session);
    ~EasyoneDialog();

private:
    Ui::EasyoneDialog *ui { Q_NULLPTR };
};

#endif // EASYONE_DIALOG_H
