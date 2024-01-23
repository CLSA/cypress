#ifndef FRAX_DIALOG_H
#define FRAX_DIALOG_H

#include "dialog_base.h"
#include "ui_frax_dialog.h"

#include "server/sessions/frax_session.h"

class FraxDialog : public DialogBase, public Ui::FraxDialog
{
    Q_OBJECT

public:
    explicit FraxDialog(QWidget *parent, QSharedPointer<FraxSession> session);
    ~FraxDialog();

private:
    Ui::FraxDialog *ui { Q_NULLPTR };
};

#endif // FRAX_DIALOG_H
