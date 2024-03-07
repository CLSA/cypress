#ifndef BLOOD_PRESSURE_DIALOG_H
#define BLOOD_PRESSURE_DIALOG_H

#include "dialog_base.h"

#include "server/sessions/bpm_session.h"

#include "widgets/bpm_manual_entry_form.h"

#include "ui_blood_pressure_dialog.h"

class BloodPressureDialog : public DialogBase, public Ui::BloodPressureDialog
{
    Q_OBJECT

public:
    BloodPressureDialog(QWidget *parent, QSharedPointer<BPMSession> session);
    ~BloodPressureDialog();

private:
    Ui::BloodPressureDialog *ui { Q_NULLPTR };

    BpmManualEntryForm* bpmManualEntryForm;
};

#endif // BLOOD_PRESSURE_DIALOG_H
