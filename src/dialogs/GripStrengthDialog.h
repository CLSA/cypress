#ifndef GRIPSTRENGTHDIALOG_H
#define GRIPSTRENGTHDIALOG_H

#include "DialogBase.h"
#include "ui_gripstrengthdialog.h"
#include "managers/grip_strength/GripStrengthManager.h"

class GripStrengthDialog : public DialogBase, public Ui::GripStrengthDialog
{
    Q_OBJECT

public:
    explicit GripStrengthDialog();
    ~GripStrengthDialog();

private:
    Ui::GripStrengthDialog *ui;
    GripStrengthManager m_manager;

    void initializeConnections() override;
    void initializeModel() override;
    void enableMeasure();
    bool handleClose();
};

#endif // GRIPSTRENGTHDIALOG_H
