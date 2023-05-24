#ifndef ECGDIALOG_H
#define ECGDIALOG_H

#include <QDialog>
#include "DialogBase.h"
#include "managers/ecg/ECGManager.h"

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
};

#endif // ECGDIALOG_H
