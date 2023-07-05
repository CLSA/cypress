#ifndef TONOMETER_DIALOG_H
#define TONOMETER_DIALOG_H

#include "dialog_base.h"
#include "ui_runnable_dialog.h"

class TonometerDialog : public DialogBase, public Ui::RunnableDialog
{
    Q_OBJECT

public:
    TonometerDialog(QWidget* parent, const CypressSession& session);
    ~TonometerDialog();

private:
    void initializeModel() override;
    void initializeConnections() override;

    Ui::RunnableDialog *ui { Q_NULLPTR };
};

#endif // TONOMETER_DIALOG_H
