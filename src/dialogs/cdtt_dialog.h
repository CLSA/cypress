#ifndef CDTT_DIALOG_H
#define CDTT_DIALOG_H

#include "dialog_base.h"
#include "ui_cdtt_dialog.h"

class CDTTDialog : public DialogBase, public Ui::CDTTDialog
{
    Q_OBJECT

public:
    CDTTDialog(QWidget* parent, const CypressSession& session);
    ~CDTTDialog();

private:
    void initializeConnections() override;

    Ui::CDTTDialog *ui { Q_NULLPTR };
};

#endif // CDTT_DIALOG_H
