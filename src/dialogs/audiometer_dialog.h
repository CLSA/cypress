#ifndef AUDIOMETER_DIALOG_H
#define AUDIOMETER_DIALOG_H

#include "dialog_base.h"
#include "ui_audiometer_dialog.h"

class AudiometerDialog : public DialogBase, public Ui::AudiometerDialog
{
    Q_OBJECT

public:
    explicit AudiometerDialog(QJsonObject inputData);
    ~AudiometerDialog();

private:
    void initializeConnections() override;

    void closeEvent(QCloseEvent* event) override;
    void userClose() override;

    Ui::AudiometerDialog *ui { Q_NULLPTR };
};

#endif // AUDIOMETER_DIALOG_H
