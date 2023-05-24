#ifndef SPIROMETERDIALOG_H
#define SPIROMETERDIALOG_H

#include "DialogBase.h"
#include "ui_runnabledialog.h"

class SpirometerDialog : public DialogBase, public Ui::RunnableDialog
{
    Q_OBJECT

public:
    explicit SpirometerDialog(QJsonObject inputData);
    ~SpirometerDialog();

private:
    void initializeConnections() override;

    Ui::RunnableDialog *ui { Q_NULLPTR };
};

#endif // SPIROMETERDIALOG_H
