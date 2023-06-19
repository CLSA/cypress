#ifndef SIGNATURE_PAD_DIALOG_H
#define SIGNATURE_PAD_DIALOG_H

#include <QDialog>
#include "dialogs/dialog_base.h"
#include "managers/signature_pad/signature_pad_manager.h"

namespace Ui {
class SignaturePadDialog;
}

class SignaturePadDialog : public DialogBase
{
    Q_OBJECT

public:
    explicit SignaturePadDialog(QJsonObject inputData);
    ~SignaturePadDialog();

private slots:
    void on_ResetButton_clicked();
    void on_SubmitButton_clicked();
    void renderSignature(const QByteArray& bytes);

protected:
    void initializeConnections() override;
    void initializeModel() override;
    //void closeEvent(QCloseEvent *event) override;

private:
    QScopedPointer<SignaturePadManager> m_manager;
    Ui::SignaturePadDialog *ui;

    void userClose() override;
    void closeEvent(QCloseEvent* event) override;
};

#endif // SIGNATURE_PAD_DIALOG_H
