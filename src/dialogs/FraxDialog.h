#ifndef FRAXDIALOG_H
#define FRAXDIALOG_H

#include "DialogBase.h"
#include "ui_runnabledialog.h"

class FraxDialog : public DialogBase, public Ui::RunnableDialog
{
    Q_OBJECT

public:
    explicit FraxDialog();
    ~FraxDialog();

private:
    void initializeConnections() override;

    Ui::RunnableDialog *ui { Q_NULLPTR };
};

#endif // FRAXDIALOG_H
