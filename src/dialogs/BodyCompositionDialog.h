#ifndef BODYCOMPOSITIONDIALOG_H
#define BODYCOMPOSITIONDIALOG_H

#include "DialogBase.h"
#include "ui_bodycompositiondialog.h"

class BodyCompositionDialog : public DialogBase, public Ui::BodyCompositionDialog
{
    Q_OBJECT

public:
    BodyCompositionDialog();
    ~BodyCompositionDialog();

private:
    void initializeConnections() override;

    Ui::BodyCompositionDialog *ui { Q_NULLPTR };
};

#endif // BODYCOMPOSITIONDIALOG_H
