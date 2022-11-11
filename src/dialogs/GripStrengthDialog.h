#ifndef GRIPSTRENGTHDIALOG_H
#define GRIPSTRENGTHDIALOG_H

#include "DialogBase.h"
#include "ui_gripstrengthdialog.h"

class GripStrengthDialog : public DialogBase, public Ui::GripStrengthDialog
{
    Q_OBJECT

public:
    explicit GripStrengthDialog(QWidget *parent = nullptr);
    ~GripStrengthDialog();

    QString getVerificationBarcode() const override;
    void setVerificationBarcode(const QString&) override;
private:
    Ui::GripStrengthDialog *ui;
    void initializeConnections() override;
    void initializeModel() override;
    void enableMeasure();
    bool handleClose();
};

#endif // GRIPSTRENGTHDIALOG_H
