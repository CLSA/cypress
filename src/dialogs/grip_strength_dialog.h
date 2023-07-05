#ifndef GRIP_STRENGTH_DIALOG_H
#define GRIP_STRENGTH_DIALOG_H

#include "dialog_base.h"
#include "ui_grip_strength_dialog.h"

class GripStrengthDialog : public DialogBase, public Ui::GripStrengthDialog
{
    Q_OBJECT

public:
    explicit GripStrengthDialog(QWidget* parent, const CypressSession& session);
    ~GripStrengthDialog();

private:
    Ui::GripStrengthDialog *ui;

    void initializeConnections() override;
    void initializeModel() override;

    void enableMeasure();
    bool handleClose();
};

#endif // GRIP_STRENGTH_DIALOG_H
