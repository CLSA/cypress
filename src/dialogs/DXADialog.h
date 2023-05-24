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
    explicit DXADialog(QJsonObject inputData);
    ~DXADialog();

protected:
    void closeEvent(QCloseEvent *event) override;
    void initializeModel() override {};
    void initializeConnections() override;

    Ui::DXADialog *ui;
    QScopedPointer<DXAManager> m_manager;

public slots:
    void dicomFilesReceived(const QStringList& dicomFilePaths);
    void dicomServerStarted();
    void dicomServerEnded();
    void dicomLogUpdate(QString line);

private slots:
    void on_openFileExplorer_released();
    void on_submitButton_clicked();
};

#endif // DXADIALOG_H
