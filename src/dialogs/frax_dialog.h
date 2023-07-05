#ifndef FRAX_DIALOG_H
#define FRAX_DIALOG_H

#include "dialog_base.h"
#include "ui_frax_dialog.h"

class FraxDialog : public DialogBase, public Ui::FraxDialog
{
    Q_OBJECT

public:
    explicit FraxDialog(QWidget* parent, const CypressSession& session);
    ~FraxDialog();

private:
    void initializeConnections() override;

    Ui::FraxDialog *ui { Q_NULLPTR };
};

#endif // FRAX_DIALOG_H
