#ifndef ECG_DIALOG_H
#define ECG_DIALOG_H

#include <QDialog>
#include "dialog_base.h"
#include "managers/ecg/ecg_manager.h"

namespace Ui {
class EcgDialog;
}

class EcgDialog : public DialogBase
{
    Q_OBJECT

public:
    explicit EcgDialog(QJsonObject inputData);
    ~EcgDialog();

protected:
    void initializeConnections() override;

private:
    Ui::EcgDialog *ui;
    QScopedPointer<ECGManager> m_manager;

    void userClose() override;
    void closeEvent(QCloseEvent *event) override;
};

#endif // ECG_DIALOG_H
