#ifndef HEARCON_DIALOG_H
#define HEARCON_DIALOG_H

#include <QDialog>

#include "dialog_base.h"
#include "server/sessions/hearcon_session.h"
#include "ui_hearcon_dialog.h"

#include "widgets/audiometer_manual_entry_form.h"


class HearconDialog : public DialogBase, public Ui::HearconDialog
{
    Q_OBJECT

public:
    explicit HearconDialog(QWidget *parent, QSharedPointer<HearconSession> session);
    ~HearconDialog();

private:
    Ui::HearconDialog *ui { Q_NULLPTR };

    AudiometerManualEntryForm* audiometerManualEntryForm;
};

#endif // HEARCON_DIALOG_H
