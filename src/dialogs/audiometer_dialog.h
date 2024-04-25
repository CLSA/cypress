#ifndef AUDIOMETER_DIALOG_H
#define AUDIOMETER_DIALOG_H

#include "dialog_base.h"
#include "server/sessions/audiometer_session.h"
#include "ui_audiometer_dialog.h"

#include "widgets/audiometer_manual_entry_form.h"

class AudiometerDialog : public DialogBase, public Ui::AudiometerDialog
{
    Q_OBJECT

public:
    explicit AudiometerDialog(QWidget *parent, QSharedPointer<AudiometerSession> session);
    ~AudiometerDialog();

private:
    Ui::AudiometerDialog *ui { Q_NULLPTR };

    AudiometerManualEntryForm* audiometerManualEntryForm;
};

#endif // AUDIOMETER_DIALOG_H
