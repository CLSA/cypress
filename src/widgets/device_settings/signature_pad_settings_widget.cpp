#include "signature_pad_settings_widget.h"
#include "ui_signature_pad_settings_widget.h"

SignaturePadSettingsWidget::SignaturePadSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignaturePadSettingsWidget)
{
    ui->setupUi(this);
}

SignaturePadSettingsWidget::~SignaturePadSettingsWidget()
{
    delete ui;
}
