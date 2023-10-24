#ifndef ECG_DIALOG_H
#define ECG_DIALOG_H

#include "dialog_base.h"
#include "server/sessions/ecg_session.h"
#include "ui_ecg_dialog.h"

#include <QDialog>

class EcgDialog : public DialogBase, public Ui::EcgDialog
{
    Q_OBJECT

public:
    explicit EcgDialog(QWidget *parent, QSharedPointer<ECGSession> session);
    ~EcgDialog();

private:
    Ui::EcgDialog* ui { Q_NULLPTR };
};

#endif // ECG_DIALOG_H
