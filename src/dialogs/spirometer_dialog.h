#ifndef SPIROMETER_DIALOG_H
#define SPIROMETER_DIALOG_H

#include "dialog_base.h"
#include "ui_runnable_dialog.h"

class SpirometerDialog : public DialogBase, public Ui::RunnableDialog
{
    Q_OBJECT

public:
    explicit SpirometerDialog(QWidget* parent, const CypressSession& session);
    ~SpirometerDialog();

private:
    void initializeConnections() override;

    void userClose() override;
    void closeEvent(QCloseEvent* event) override;

    Ui::RunnableDialog *ui { Q_NULLPTR };
};

#endif // SPIROMETER_DIALOG_H
