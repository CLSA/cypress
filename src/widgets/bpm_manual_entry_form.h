#ifndef BPM_MANUAL_ENTRY_FORM_H
#define BPM_MANUAL_ENTRY_FORM_H

#include <QDialog>

namespace Ui {
class BpmManualEntryForm;
}

class BpmManualEntryForm : public QDialog
{
    Q_OBJECT

public:
    explicit BpmManualEntryForm(QWidget *parent = nullptr);
    ~BpmManualEntryForm();

    void clearForm();

signals:
    void manualBpmMeasure(
        const int systolic,
        const int diastolic,
        const int pulse);

private:
    Ui::BpmManualEntryForm *ui;

    bool validInputs();

    qint32 m_systolic { -1 };
    qint32 m_diastolic { -1 };
    qint32 m_pulse { -1 };
};

#endif // BPM_MANUAL_ENTRY_FORM_H
