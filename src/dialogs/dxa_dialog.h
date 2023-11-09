#ifndef DXA_DIALOG_H
#define DXA_DIALOG_H

#include "dialog_base.h"
#include "server/sessions/dxa/dxa_session.h"

#include "ui_dxa_dialog.h"

#include <QWidget>

class DXADialog : public DialogBase, public Ui::DXADialog
{
    Q_OBJECT

public:
    explicit DXADialog(QWidget *parent, QSharedPointer<DXASession> session);
    ~DXADialog();

public slots:
    void dicomFilesReceived(const QStringList& dicomFilePaths);
    void dicomServerStarted();
    void dicomServerEnded();
    void dicomLogUpdate(QString line);

private:
    Ui::DXADialog *ui { Q_NULLPTR };
};

#endif // DXA_DIALOG_H
