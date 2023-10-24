#ifndef WEIGH_SCALE_DIALOG_H
#define WEIGH_SCALE_DIALOG_H

#include "dialog_base.h"
#include "ui_weigh_scale_dialog.h"

#include "server/sessions/weigh_scale_session.h"

class WeighScaleDialog : public DialogBase, public Ui::WeighScaleDialog
{
    Q_OBJECT

public:
    explicit WeighScaleDialog(QWidget *parent, QSharedPointer<WeighScaleSession> session);
    ~WeighScaleDialog();


private:
    void initializeModel() override;
    void initializeConnections() override;

    Ui::WeighScaleDialog *ui { Q_NULLPTR };
};

#endif // WEIGH_SCALE_DIALOG_H
