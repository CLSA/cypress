#ifndef GRIPSTRENGTHDIALOG_H
#define GRIPSTRENGTHDIALOG_H

#include "DialogBase.h"
#include "ui_gripstrengthdialog.h"

class GripStrengthDialog : public DialogBase, public Ui::GripStrengthDialog
{
    Q_OBJECT

public:
    explicit GripStrengthDialog();
    ~GripStrengthDialog();

private:
    Ui::GripStrengthDialog *ui;
    void initializeConnections() override;
    void initializeModel() override;
    void enableMeasure();
    bool handleClose();
};

#endif // GRIPSTRENGTHDIALOG_H
