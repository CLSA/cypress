#ifndef BLOOD_PRESSURE_DIALOG_H
#define BLOOD_PRESSURE_DIALOG_H

#include "dialog_base.h"

#include "managers/blood_pressure/blood_pressure_manager.h"

#include "server/sessions/bpm_session.h"

#include "widgets/bpm_manual_entry_form.h"

#include "ui_blood_pressure_dialog.h"

class BloodPressureDialog : public DialogBase, public Ui::BloodPressureDialog
{
    Q_OBJECT

public:
    BloodPressureDialog(QWidget *parent, QSharedPointer<BPMSession> session);
    ~BloodPressureDialog();

public slots:
    void deviceStateChanged(const BloodPressureManager::State state);

    void deviceCycled(const quint8 cycle);
    void deviceCleared();

    void cuffPressureChanged(const int cuffPressure);

private:
    Ui::BloodPressureDialog *ui { Q_NULLPTR };

    BpmManualEntryForm* bpmManualEntryForm;

    void deviceConnected();
    void deviceDisconnected();
    void deviceCycling();
    void deviceReady();
    void deviceMeasuring();
    void deviceStopped();
    void deviceComplete();
};

#endif // BLOOD_PRESSURE_DIALOG_H
