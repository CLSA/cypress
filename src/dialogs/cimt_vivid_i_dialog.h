#ifndef CIMT_VIVID_I_DIALOG_H
#define CIMT_VIVID_I_DIALOG_H

#include <QWidget>
#include "dialog_base.h"
#include "server/sessions/ultrasound_session.h"

namespace Ui {
class CimtVividiDialog;
}

class CimtVividiDialog : public DialogBase
{
    Q_OBJECT

public:
    explicit CimtVividiDialog(QWidget *parent, QSharedPointer<UltrasoundSession> session);
    ~CimtVividiDialog();

private:
    Ui::CimtVividiDialog *ui;
};

#endif // CIMT_VIVID_I_DIALOG_H
