#ifndef CHOICE_REACTION_DIALOG_H
#define CHOICE_REACTION_DIALOG_H

#include "dialog_base.h"
#include "ui_runnable_dialog.h"

class ChoiceReactionDialog : public DialogBase, public Ui::RunnableDialog
{
    Q_OBJECT

public:
    ChoiceReactionDialog(QWidget* parent, const CypressSession& session);
    ~ChoiceReactionDialog();

private:
    void initializeConnections() override;

    void userClose() override;
    void closeEvent(QCloseEvent* event) override;

    Ui::RunnableDialog *ui { Q_NULLPTR };
};

#endif // CHOICE_REACTION_DIALOG_H
