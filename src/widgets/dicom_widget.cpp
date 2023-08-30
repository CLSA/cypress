#include "dicom_widget.h"
#include "ui_dicom_widget.h"

#include <QDebug>
#include <QRegExpValidator>
#include <QMessageBox>

DicomWidget::DicomWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DicomWidget)
{
    QRegExpValidator* aeTitleValidator = new QRegExpValidator(
        QRegExp("^[a-zA-Z0-9]{1,16}$"),
        this
    );
    QRegExpValidator* portValidator = new QRegExpValidator(
        QRegExp("^(6553[0-5]|655[0-2]\\d|65[0-4]\\d{2}|6[0-4]\\d{3}|[1-5]\\d{4}|[1-9]\\d{3}|[1-9]\\d{2}|102[4-9])$"),
        this
    );

    ui->setupUi(this);

    ui->portValue->setValidator(portValidator);
    ui->aeTitleValue->setValidator(aeTitleValidator);

    ui->aeTitleValue->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    ui->portValue->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    connect(ui->saveDicomSettings, &QPushButton::clicked, this, &DicomWidget::onSaveButtonClicked);
}

DicomWidget::~DicomWidget()
{
    delete ui;
}

void DicomWidget::setDicomLabels(const QString& aeTitle, const QString& hostname, const QString& port)
{
    ui->aeTitleValue->blockSignals(true);
    ui->portValue->blockSignals(true);

    m_aeTitle = aeTitle;
    m_port = port;
    m_hostname = hostname;

    ui->aeTitleValue->setText(m_aeTitle);
    ui->hostnameValue->setText(m_hostname);
    ui->portValue->setText(m_port);

    ui->aeTitleValue->blockSignals(false);
    ui->portValue->blockSignals(false);
}

void DicomWidget::on_aeTitleValue_textChanged(const QString &arg1)
{
    QString upperText = arg1.toUpper();

    if (arg1 != upperText) {
        ui->aeTitleValue->blockSignals(true);
        ui->aeTitleValue->setText(upperText);
        ui->aeTitleValue->blockSignals(false);
    }

    ui->saveDicomSettings->setEnabled(true);
}


void DicomWidget::on_portValue_textChanged(const QString &arg1)
{
    ui->saveDicomSettings->setEnabled(true);
}

void DicomWidget::onSaveButtonClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,
        "Confirm Action", "Are you sure you want to change the DICOM server settings? This will cause the server to restart.",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        m_aeTitle = ui->aeTitleValue->text();
        m_port = ui->portValue->text();

        emit updateDicomSettings(m_aeTitle, m_port);
    } else {
        setDicomLabels(m_aeTitle, m_hostname, m_port);
    }

    ui->saveDicomSettings->setEnabled(false);
}

