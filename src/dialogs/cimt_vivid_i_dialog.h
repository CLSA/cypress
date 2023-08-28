#ifndef CIMT_VIVID_I_DIALOG_H
#define CIMT_VIVID_I_DIALOG_H

#include "dialog_base.h"
#include <QWidget>

namespace Ui {
class CimtVividiDialog;
}

class CimtVividiDialog : public DialogBase
{
    Q_OBJECT

public:
    explicit CimtVividiDialog(const CypressSession& session, QWidget *parent = nullptr);
    ~CimtVividiDialog();

private:
    Ui::CimtVividiDialog *ui;
};

#endif // CIMT_VIVID_I_DIALOG_H
