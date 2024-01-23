#ifndef BODY_COMPOSITION_DIALOG_H
#define BODY_COMPOSITION_DIALOG_H

#include "dialog_base.h"
#include "ui_body_composition_dialog.h"

class BodyCompositionDialog : public DialogBase, public Ui::BodyCompositionDialog
{
    Q_OBJECT

public:
    BodyCompositionDialog(QWidget *parent, QSharedPointer<CypressSession> session);
    ~BodyCompositionDialog();

private:
    Ui::BodyCompositionDialog *ui { Q_NULLPTR };
};

#endif // BODY_COMPOSITION_DIALOG_H
