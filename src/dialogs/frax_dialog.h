#ifndef FRAX_DIALOG_H
#define FRAX_DIALOG_H

#include "dialog_base.h"
#include "ui_runnable_dialog.h"

class FraxDialog : public DialogBase, public Ui::RunnableDialog
{
    Q_OBJECT

public:
    explicit FraxDialog(QJsonObject inputData);
    ~FraxDialog();

private:
    void initializeConnections() override;

    void userClose() override;
    void closeEvent(QCloseEvent *event) override;

    Ui::RunnableDialog *ui { Q_NULLPTR };
};

#endif // FRAX_DIALOG_H
