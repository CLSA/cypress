#ifndef VIVID_IQ_DIALOG_H
#define VIVID_IQ_DIALOG_H


#include "dialog_base.h"
#include "cypress_session.h"

#include "ui_vivid_iq_dialog.h"

class VividIQDialog : public DialogBase, public Ui::VividIQDialog
{
    Q_OBJECT

public:
    explicit VividIQDialog(QWidget *parent, QSharedPointer<CypressSession> session);
    ~VividIQDialog();

private:
    Ui::VividIQDialog *ui;
};

#endif // VIVID_IQ_DIALOG_H
