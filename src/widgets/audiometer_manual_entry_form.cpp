#include "audiometer_manual_entry_form.h"
#include "ui_audiometer_manual_entry_form.h"

#include <QMessageBox>

AudiometerManualEntryForm::AudiometerManualEntryForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AudiometerManualEntryForm)
{
    ui->setupUi(this);

    setWindowTitle("Manual Entry");

    ui->levelLineEdit->setValidator(new QIntValidator(0, 500));
    ui->okButton->setEnabled(false);

    QStringList testList = { "---", "1 kHz Test", "500 Hz", "1 kHz", "2 kHz", "3 kHz", "4 kHz", "5 kHz", "6 kHz", "8 kHz" };
    ui->testComboBox->addItems(testList);

    m_test = "---";
    m_side = "---";

    QStringList sideList = { "---", "left", "right" };
    ui->sideComboBox->addItems(sideList);

    connect(ui->testComboBox, &QComboBox::currentTextChanged, this, [this](const QString& test) {
        m_test = test;
    });

    connect(ui->sideComboBox, &QComboBox::currentTextChanged, this, [this](const QString& side) {
        m_side = side;
    });

    connect(ui->levelLineEdit, &QLineEdit::textEdited, this, [=](const QString& level) {
        bool ok = false;
        m_level = level.toInt(&ok);
        if (!ok)
            return;

        ui->okButton->setEnabled(validInputs());
    });

    connect(ui->okButton, &QPushButton::clicked, this, [=]() {
        ui->okButton->setEnabled(false);

        if (validInputs()) {
            emit manualAudiometerMeasurement(m_side, m_test, m_level, m_level <= 40);
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

void AudiometerManualEntryForm::clearForm() {
    ui->sideComboBox->setCurrentIndex(0);
    ui->testComboBox->setCurrentIndex(0);
    //ui->sideComboBox->clear();
    //ui->testComboBox->clear();
    ui->levelLineEdit->clear();
    ui->sideComboBox->setFocus();

    m_level = -1;
    m_side = "";
    m_test = "";
}

bool AudiometerManualEntryForm::validInputs() {
    if (ui->sideComboBox->currentText() == "---") {
        return false;
    }

    if (ui->testComboBox->currentText() == "---") {
        return false;
    }

    return m_level >= 0 &&
           (!m_test.isNull() && !m_test.isEmpty()) &&
           (!m_side.isNull() && !m_side.isEmpty());
}

AudiometerManualEntryForm::~AudiometerManualEntryForm()
{
    delete ui;
}
