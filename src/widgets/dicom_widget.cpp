#include "dicom_widget.h"
#include "ui_dicom_widget.h"

#include <QDebug>
#include <QRegExpValidator>
#include <QMessageBox>
#include <QHostInfo>

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

    connect(ui->aeTitleValue, &QLineEdit::editingFinished, this, [=] {
        emit aeTitleChanged(ui->aeTitleValue->text());
    });

    connect(ui->portValue, &QLineEdit::editingFinished, this, [=] {
        emit portChanged(ui->portValue->text());
    });
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

    if (hostname.isNull() || hostname.isEmpty())
    {
        m_hostname = QHostInfo::localHostName();
    }
    else
    {
        m_hostname = hostname;
    }

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
}




