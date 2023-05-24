#ifndef TONOMETERDIALOG_H
#define TONOMETERDIALOG_H

#include "DialogBase.h"
#include "ui_runnabledialog.h"

class TonometerDialog : public DialogBase, public Ui::RunnableDialog
{
    Q_OBJECT

public:
    TonometerDialog(QJsonObject inputData);
    ~TonometerDialog();

private:
    void initializeModel() override;
    void initializeConnections() override;

    Ui::RunnableDialog *ui { Q_NULLPTR };
};

#endif // TONOMETERDIALOG_H
