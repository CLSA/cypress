#ifndef WEIGHT_SCALE_MANUAL_ENTRY_FORM_H
#define WEIGHT_SCALE_MANUAL_ENTRY_FORM_H

#include <QDialog>

namespace Ui {
class WeightScaleManualEntryForm;
}

class WeightScaleManualEntryForm : public QDialog
{
    Q_OBJECT

public:
    explicit WeightScaleManualEntryForm(QWidget *parent = nullptr);
    ~WeightScaleManualEntryForm();

    void clearForm();

signals:
    void manualWeightEntry(const double weight);

private:
    double m_weight { -1 };


    bool validInputs();

    Ui::WeightScaleManualEntryForm *ui;
};

#endif // WEIGHT_SCALE_MANUAL_ENTRY_FORM_H
