#ifndef ECG_DIALOG_H
#define ECG_DIALOG_H

#include <QDialog>
#include "dialog_base.h"
#include "ui_ecg_dialog.h"


class EcgDialog : public DialogBase, public Ui::EcgDialog
{
    Q_OBJECT

public:
    explicit EcgDialog(QWidget* parent, const CypressSession& session);
    ~EcgDialog();

private:
    Ui::EcgDialog* ui { Q_NULLPTR };
};

#endif // ECG_DIALOG_H
