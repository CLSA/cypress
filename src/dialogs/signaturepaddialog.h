#ifndef SIGNATUREPADDIALOG_H
#define SIGNATUREPADDIALOG_H

#include <QDialog>
#include "dialogs/DialogBase.h"
#include "managers/signature_pad/signaturepadmanager.h"

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
};

#endif // SIGNATUREPADDIALOG_H
