#ifndef SIGNATURE_PAD_DIALOG_H
#define SIGNATURE_PAD_DIALOG_H

#include <QDialog>
#include "dialogs/dialog_base.h"

namespace Ui {
class SignaturePadDialog;
}

class SignaturePadDialog : public DialogBase
{
    Q_OBJECT

public:
    explicit SignaturePadDialog(QWidget *parent, QSharedPointer<CypressSession> session);
    ~SignaturePadDialog();

private slots:
    void renderSignature(const QByteArray& bytes);

private:
    Ui::SignaturePadDialog *ui;
};

#endif // SIGNATURE_PAD_DIALOG_H
