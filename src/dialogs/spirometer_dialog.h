#ifndef SPIROMETER_DIALOG_H
#define SPIROMETER_DIALOG_H

#include "dialog_base.h"
#include "ui_spirometer_dialog.h"


class SpirometerDialog : public DialogBase, public Ui::SpirometerDialog
{
    Q_OBJECT

public:
    explicit SpirometerDialog(QWidget* parent, const CypressSession& session);
    ~SpirometerDialog();

private:
    void initializeConnections() override;

    Ui::SpirometerDialog *ui { Q_NULLPTR };
};

#endif // SPIROMETER_DIALOG_H
