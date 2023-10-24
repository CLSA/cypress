#ifndef RETINAL_CAMERA_DIALOG_H
#define RETINAL_CAMERA_DIALOG_H

#include "dialogs/dialog_base.h"
#include "server/sessions/retinal_camera_session.h"

#include <QDialog>

namespace Ui {
    class RetinalCameraDialog;
}

class RetinalCameraDialog : public DialogBase
{
    Q_OBJECT

public:
    explicit RetinalCameraDialog(QWidget *parent, QSharedPointer<RetinalCameraSession> session);
    ~RetinalCameraDialog();

private:
    void initializeModel() override;
    void initializeConnections() override;

    Ui::RetinalCameraDialog *ui;
};

#endif // RETINAL_CAMERA_DIALOG_H
