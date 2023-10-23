#ifndef TONOMETER_DIALOG_H
#define TONOMETER_DIALOG_H

#include "dialog_base.h"
#include "ui_tonometer_dialog.h"

class TonometerDialog : public DialogBase, public Ui::TonometerDialog
{
    Q_OBJECT

public:
    TonometerDialog(QWidget* parent, const CypressSession& session);
    ~TonometerDialog();

private:
    void initializeModel() override;
    void initializeConnections() override;

    Ui::TonometerDialog *ui { Q_NULLPTR };
};

#endif // TONOMETER_DIALOG_H
