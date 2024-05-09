#ifndef OCT_DIALOG_H
#define OCT_DIALOG_H

#include <QDialog>

#include "dialog_base.h"
#include "server/sessions/oct_session.h"


namespace Ui {
class OCTDialog;
}

class OCTDialog : public DialogBase
{
    Q_OBJECT

public:
    explicit OCTDialog(QWidget *parent, QSharedPointer<OCTSession> session);
    ~OCTDialog();

private:
    Ui::OCTDialog *ui;
};

#endif // OCT_DIALOG_H
