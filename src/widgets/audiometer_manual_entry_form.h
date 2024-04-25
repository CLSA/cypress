#ifndef AUDIOMETER_MANUAL_ENTRY_FORM_H
#define AUDIOMETER_MANUAL_ENTRY_FORM_H

#include <QDialog>

namespace Ui {
class AudiometerManualEntryForm;
}

class AudiometerManualEntryForm : public QDialog
{
    Q_OBJECT

public:
    explicit AudiometerManualEntryForm(QWidget *parent = nullptr);
    ~AudiometerManualEntryForm();

    void clearForm();
    bool validInputs();

signals:;
    void manualAudiometerMeasurement(const QString side, const QString test, const int value, const bool pass);

private:
    QString m_test;
    QString m_side;
    int m_level;


    Ui::AudiometerManualEntryForm *ui;
};

#endif // AUDIOMETER_MANUAL_ENTRY_FORM_H
