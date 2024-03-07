#include "weight_scale_manual_entry_form.h"
#include "ui_weight_scale_manual_entry_form.h"

#include <QDoubleValidator>
#include <QMessageBox>

WeightScaleManualEntryForm::WeightScaleManualEntryForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WeightScaleManualEntryForm)
{
    ui->setupUi(this);
    setWindowTitle("Manual Entry");

    ui->weightValue->setValidator(new QDoubleValidator(0.1, 1000, 1));

    connect(ui->weightValue, &QLineEdit::textEdited, this, [=](const QString& weight) {
        bool ok = false;
        m_weight = weight.toDouble(&ok);
        if (!ok)
            return;
        ui->okButton->setEnabled(validInputs());
    });

    connect(ui->okButton, &QPushButton::clicked, this, [=]() {
        ui->okButton->setEnabled(false);

        if (validInputs()) {
            emit manualWeightEntry(m_weight);
            hide();
            clearForm();
        }
    });

    connect(ui->cancelButton, &QPushButton::clicked, this, [=]() {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Cancel", "Are you sure you wish to cancel?");
        if (reply == QMessageBox::Yes) {
            close();
        }
    });
}

void WeightScaleManualEntryForm::clearForm() {
    ui->weightValue->clear();
    ui->weightValue->setFocus();

    m_weight = -1;
}

bool WeightScaleManualEntryForm::validInputs() {
    return (m_weight > 0);
}

WeightScaleManualEntryForm::~WeightScaleManualEntryForm()
{
    delete ui;
}
