#include "body_composition_settings_widget.h"
#include "ui_body_composition_settings_widget.h"

BodyCompositionSettingsWidget::BodyCompositionSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BodyCompositionSettingsWidget)
{
    ui->setupUi(this);
}

BodyCompositionSettingsWidget::~BodyCompositionSettingsWidget()
{
    delete ui;
}
