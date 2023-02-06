#ifndef ECGDIALOG_H
#define ECGDIALOG_H

#include "DialogBase.h"
#include "ui_runnabledialog.h"

class ECGDialog : public DialogBase, public Ui::RunnableDialog
{
    Q_OBJECT

public:
    explicit ECGDialog();
    ~ECGDialog();

private:
    void initializeConnections() override;

    Ui::RunnableDialog *ui { Q_NULLPTR };
};

#endif // ECGDIALOG_H
