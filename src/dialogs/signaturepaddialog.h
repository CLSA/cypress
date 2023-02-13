#ifndef SIGNATUREPADDIALOG_H
#define SIGNATUREPADDIALOG_H

#include <QDialog>
#include "dialogs/DialogBase.h"

namespace Ui {
class SignaturePadDialog;
}

class SignaturePadDialog : public DialogBase
{
    Q_OBJECT

public:
    explicit SignaturePadDialog(QString uuid);
    ~SignaturePadDialog();

private slots:
    void on_graphicsView_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint);

protected:
    void initializeConnections() override;
    void initializeModel() override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::SignaturePadDialog *ui;
};

#endif // SIGNATUREPADDIALOG_H
