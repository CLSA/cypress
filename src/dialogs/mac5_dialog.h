#ifndef MAC5_DIALOG_H
#define MAC5_DIALOG_H

#include "dialogs/dialog_base.h"
#include "server/sessions/mac5_session.h"

#include "ui_mac5_dialog.h"



class Mac5Dialog : public DialogBase, public Ui::Mac5Dialog
{
    Q_OBJECT

public:
    Mac5Dialog(QWidget *parent, QSharedPointer<Mac5Session> session);
    ~Mac5Dialog();

private:
    Ui::Mac5Dialog *ui { Q_NULLPTR} ;
};

#endif // MAC5_DIALOG_H
