#ifndef CHOICEREACTIONDIALOG_H
#define CHOICEREACTIONDIALOG_H

#include "DialogBase.h"
#include "ui_runnabledialog.h"

class ChoiceReactionDialog : public DialogBase, public Ui::RunnableDialog
{
    Q_OBJECT

public:
    ChoiceReactionDialog(QJsonObject inputData);
    ~ChoiceReactionDialog();

private:
    void initializeConnections() override;

    Ui::RunnableDialog *ui { Q_NULLPTR };
};

#endif // CHOICEREACTIONDIALOG_H
