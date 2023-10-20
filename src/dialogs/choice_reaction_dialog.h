#ifndef CHOICE_REACTION_DIALOG_H
#define CHOICE_REACTION_DIALOG_H

#include "dialog_base.h"
#include "ui_choice_reaction_dialog.h"

class ChoiceReactionDialog : public DialogBase, public Ui::ChoiceReactionDialog
{
    Q_OBJECT

public:
    ChoiceReactionDialog(QWidget* parent, const CypressSession& session);
    ~ChoiceReactionDialog();

private:
    void initializeConnections() override;

    Ui::ChoiceReactionDialog *ui { Q_NULLPTR };
};

#endif // CHOICE_REACTION_DIALOG_H
