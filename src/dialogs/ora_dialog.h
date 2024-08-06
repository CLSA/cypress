#ifndef ORA_DIALOG_H
#define ORA_DIALOG_H

#include "dialog_base.h"
#include "server/sessions/ora_session.h"

#include "ui_ora_dialog.h"

#include <QDialog>


class ORADialog : public DialogBase, public Ui::ORADialog
{
    Q_OBJECT

public:
    explicit ORADialog(QWidget *parent, QSharedPointer<ORASession> session);
    ~ORADialog();

private:
    Ui::ORADialog *ui { Q_NULLPTR };
};

#endif // ORA_DIALOG_H
