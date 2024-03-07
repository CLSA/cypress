#include "bpm_manual_entry_form.h"
#include "ui_bpm_manual_entry_form.h"

#include <QMessageBox>
#include <QDoubleValidator>
#include <QIntValidator>

BpmManualEntryForm::BpmManualEntryForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BpmManualEntryForm)
{
    ui->setupUi(this);

    setWindowTitle("Manual Entry");

    ui->systolicValue->setValidator(new QIntValidator(0, 500));
    ui->diastolicValue->setValidator(new QIntValidator(0, 500));
    ui->pulseValue->setValidator(new QIntValidator(0, 500));
    ui->okButton->setEnabled(false);

    connect(ui->systolicValue, &QLineEdit::textEdited, this, [=](const QString& systolic) {
        bool ok = false;
        m_systolic = systolic.toInt(&ok);
        if (!ok)
            return;

        ui->okButton->setEnabled(validInputs());
    });

    connect(ui->diastolicValue, &QLineEdit::textEdited, this, [=](const QString& diastolic) {
        bool ok = false;
        m_diastolic = diastolic.toInt(&ok);
        if (!ok)
            return;

        ui->okButton->setEnabled(validInputs());
    });

    connect(ui->pulseValue, &QLineEdit::textEdited, this, [=](const QString& pulse) {
        bool ok = false;
        m_pulse = pulse.toInt(&ok);
        if (!ok)
            return;

        ui->okButton->setEnabled(validInputs());
    });

    connect(ui->okButton, &QPushButton::clicked, this, [=]() {
        ui->okButton->setEnabled(false);
        if (validInputs()) {
            emit manualBpmMeasure(m_systolic, m_diastolic, m_pulse);
            hide();
            clearForm();
        }
    });

    connect(ui->cancelButton, &QPushButton::clicked, this, [=]() {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Cancel", "Are you sure you wish to cancel?");
        if (reply == QMessageBox::Yes) {
            hide();
            clearForm();
        }
    });
}

void BpmManualEntryForm::clearForm() {
    ui->systolicValue->clear();
    ui->diastolicValue->clear();
    ui->pulseValue->clear();

    m_systolic = -1;
    m_diastolic = -1;
    m_pulse = -1;

    ui->systolicValue->setFocus();
}

bool BpmManualEntryForm::validInputs() {
    return (m_systolic >= 0 && m_diastolic >= 0 && m_pulse >= 0);
}

BpmManualEntryForm::~BpmManualEntryForm()
{
    delete ui;
}
