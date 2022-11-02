#ifndef DXADIALOG_H
#define DXADIALOG_H

#include <QWidget>
#include "DialogBase.h"
#include "managers/dxa/DXAManager.h"

namespace Ui {
class DXADialog;
}

class DXADialog : public DialogBase
{
    Q_OBJECT

public:
    explicit DXADialog(DXAManager* manager, QWidget *parent = nullptr);
    ~DXADialog();

    QString getVerificationBarcode() const override { return ""; };
    void setVerificationBarcode(const QString&) override { } ;

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void dicomServerStarted();
    void dicomServerEnded();
    void dicomLogUpdate(QString line);

private slots:
    void on_openFileExplorer_released();

private:
    std::unique_ptr<DXAManager> m_manager;
    void initializeModel() override {};
    void initializeConnections() override;

    Ui::DXADialog *ui;
};

#endif // DXADIALOG_H
