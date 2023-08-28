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
