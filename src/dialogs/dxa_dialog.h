#ifndef DXA_DIALOG_H
#define DXA_DIALOG_H

#include <QWidget>
#include "dialog_base.h"
#include "managers/dxa/dxa_manager.h"

namespace Ui {
class DXADialog;
}

class DXADialog : public DialogBase
{
    Q_OBJECT

public:
    explicit DXADialog(QWidget* parent, const CypressSession& session);
    ~DXADialog();

private:
    void userClose() override;
    void closeEvent(QCloseEvent *event) override;

protected:
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

#endif // DXA_DIALOG_H
