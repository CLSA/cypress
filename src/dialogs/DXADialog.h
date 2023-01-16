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

protected:
    void closeEvent(QCloseEvent *event) override;
    void initializeModel() override {};
    void initializeConnections() override;

    Ui::DXADialog *ui;
    std::unique_ptr<DXAManager> m_manager;

public slots:
    void dicomFilesReceived(QStringList& dicomFilePaths);
    void dicomServerStarted();
    void dicomServerEnded();
    void dicomLogUpdate(QString line);

private slots:
    void on_openFileExplorer_released();
};

#endif // DXADIALOG_H
