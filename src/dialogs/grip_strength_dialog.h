#ifndef GRIP_STRENGTH_DIALOG_H
#define GRIP_STRENGTH_DIALOG_H

#include "dialog_base.h"
#include "server/sessions/grip_strength_session.h"

#include "ui_grip_strength_dialog.h"

class GripStrengthDialog : public DialogBase, public Ui::GripStrengthDialog
{
    Q_OBJECT

public:
    explicit GripStrengthDialog(QWidget *parent, QSharedPointer<GripStrengthSession> session);
    ~GripStrengthDialog();

private:
    Ui::GripStrengthDialog *ui;
};

#endif // GRIP_STRENGTH_DIALOG_H
