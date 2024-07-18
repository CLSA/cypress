#ifndef WATCH_BP_DIALOG_H
#define WATCH_BP_DIALOG_H

#include "dialog_base.h"
#include "server/sessions/watch_bp_session.h"
#include "ui_watch_bp_dialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

class WatchBPDialog : public DialogBase, public Ui::WatchBPDialog
{
    Q_OBJECT

public:
    explicit WatchBPDialog(QWidget *parent, QSharedPointer<WatchBPSession> session);
    ~WatchBPDialog();

private:
    Ui::WatchBPDialog *ui;
};

#endif // WATCH_BP_DIALOG_H
