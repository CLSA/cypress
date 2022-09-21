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

private:
    Ui::GripStrengthDialog *ui;
};

#endif // GRIPSTRENGTHDIALOG_H
