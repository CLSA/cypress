#ifndef BLOOD_PRESSURE_DIALOG_H
#define BLOOD_PRESSURE_DIALOG_H

#include "dialog_base.h"
#include "ui_blood_pressure_dialog.h"

class BloodPressureDialog : public DialogBase, public Ui::BloodPressureDialog
{
    Q_OBJECT

public:
    BloodPressureDialog(QWidget* parent, const CypressSession& session);
    ~BloodPressureDialog();

private:
    void initializeModel() override;
    void initializeConnections() override;

    Ui::BloodPressureDialog *ui { Q_NULLPTR };
};

#endif // BLOOD_PRESSURE_DIALOG_H
