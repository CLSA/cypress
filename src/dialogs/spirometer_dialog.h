#ifndef SPIROMETER_DIALOG_H
#define SPIROMETER_DIALOG_H

#include "dialog_base.h"
#include "ui_spirometer_dialog.h"

#include "server/sessions/spirometer_session.h"

class SpirometerDialog : public DialogBase, public Ui::SpirometerDialog
{
    Q_OBJECT

public:
    explicit SpirometerDialog(QWidget *parent, QSharedPointer<SpirometerSession> session);
    ~SpirometerDialog();

private:
    Ui::SpirometerDialog *ui { Q_NULLPTR };
};

#endif // SPIROMETER_DIALOG_H
