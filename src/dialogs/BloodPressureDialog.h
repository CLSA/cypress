#ifndef BLOODPRESSUREDIALOG_H
#define BLOODPRESSUREDIALOG_H

#include "DialogBase.h"
#include "ui_bloodpressuredialog.h"

class BloodPressureDialog : public DialogBase, public Ui::BloodPressureDialog
{
    Q_OBJECT

public:
    BloodPressureDialog(QWidget *parent = Q_NULLPTR);
    ~BloodPressureDialog();

private:
    void initializeModel() override;
    void initializeConnections() override;

    Ui::BloodPressureDialog *ui { Q_NULLPTR };
};

#endif // BLOODPRESSUREDIALOG_H
