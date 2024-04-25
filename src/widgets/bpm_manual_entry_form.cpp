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

    ui->okButton->setEnabled(false);

    connect(ui->systolicValue, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](const int systolic) {
        m_systolic = systolic;
        ui->okButton->setEnabled(validInputs());
    });

    connect(ui->diastolicValue, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](const int diastolic) {
        m_diastolic = diastolic;
        ui->okButton->setEnabled(validInputs());
    });

    connect(ui->pulseValue, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](const int pulse) {
        m_pulse = pulse;
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
