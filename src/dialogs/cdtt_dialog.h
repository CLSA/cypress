#ifndef CDTT_DIALOG_H
#define CDTT_DIALOG_H

#include "dialog_base.h"
#include "ui_runnable_dialog.h"

class CDTTDialog : public DialogBase, public Ui::RunnableDialog
{
    Q_OBJECT

public:
    CDTTDialog(QJsonObject inputData);
    ~CDTTDialog();

private:
    void initializeConnections() override;

    void userClose() override;
    void closeEvent(QCloseEvent* event) override;

    Ui::RunnableDialog *ui { Q_NULLPTR };
};

#endif // CDTT_DIALOG_H
