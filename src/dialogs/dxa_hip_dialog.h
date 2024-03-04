#ifndef DXA_HIP_DIALOG_H
#define DXA_HIP_DIALOG_H

#include "dialog_base.h"
#include "server/sessions/dxa/dxa_hip_session.h"
#include "ui_dxa_hip_dialog.h"

#include <QWidget>

namespace Ui {
class DxaHipDialog;
}

class DxaHipDialog: public DialogBase, public Ui::DxaHipDialog
{
    Q_OBJECT

public:
    explicit DxaHipDialog(QWidget *parent, QSharedPointer<DxaHipSession> session);
    ~DxaHipDialog();

private:
    Ui::DxaHipDialog *ui { Q_NULLPTR };

    void initMeasurementTable();
    void initManager();

};

#endif // DXA_HIP_DIALOG_H
