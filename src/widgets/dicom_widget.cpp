#include "dicom_widget.h"
#include "ui_dicom_widget.h"

DicomWidget::DicomWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DicomWidget)
{
    ui->setupUi(this);
}

DicomWidget::~DicomWidget()
{
    delete ui;
}

void DicomWidget::setDicomLabels(const QString& aeTitle, const QString& hostname, const QString& port)
{
    ui->aeTitleValue->setText(aeTitle);
    ui->hostnameValue->setText(hostname);
    ui->portValue->setText(port);
}
