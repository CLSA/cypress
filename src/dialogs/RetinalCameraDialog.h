#ifndef RETINALCAMERADIALOG_H
#define RETINALCAMERADIALOG_H

#include "dialogs/DialogBase.h"
#include <QDialog>

namespace Ui {
    class RetinalCameraDialog;
}

class RetinalCameraDialog : public DialogBase
{
    Q_OBJECT

public:
    explicit RetinalCameraDialog(QWidget *parent = nullptr);
    ~RetinalCameraDialog();

private:
    Ui::RetinalCameraDialog *ui;

    void initializeModel() override;
    void initializeConnections() override;
    bool handleClose();
};

#endif // RETINALCAMERADIALOG_H
