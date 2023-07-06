#ifndef DXA_DIALOG_H
#define DXA_DIALOG_H

#include <QWidget>
#include "dialog_base.h"
#include "ui_dxa_dialog.h"


class DXADialog : public DialogBase, public Ui::DXADialog
{
    Q_OBJECT

public:
    explicit DXADialog(QWidget* parent, const CypressSession& session);
    ~DXADialog();

public slots:
    void dicomFilesReceived(const QStringList& dicomFilePaths);
    void dicomServerStarted();
    void dicomServerEnded();
    void dicomLogUpdate(QString line);

private slots:
    void on_openFileExplorer_released();
    void on_submitButton_clicked();

private:
    Ui::DXADialog *ui { Q_NULLPTR };
};

#endif // DXA_DIALOG_H
