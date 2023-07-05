#ifndef RETINAL_CAMERA_DIALOG_H
#define RETINAL_CAMERA_DIALOG_H

#include "dialogs/dialog_base.h"
#include <QDialog>

namespace Ui {
    class RetinalCameraDialog;
}

class RetinalCameraDialog : public DialogBase
{
    Q_OBJECT

public:
    explicit RetinalCameraDialog(QWidget* parent, const CypressSession& session);
    ~RetinalCameraDialog();



private:
    Ui::RetinalCameraDialog *ui;

    void userClose() override;
    void closeEvent(QCloseEvent* event) override;

    void initializeModel() override;
    void initializeConnections() override;
};

#endif // RETINAL_CAMERA_DIALOG_H
