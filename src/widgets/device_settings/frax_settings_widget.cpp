#include "frax_settings_widget.h"
#include "ui_frax_settings_widget.h"

FraxSettingsWidget::FraxSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FraxSettingsWidget)
{
    ui->setupUi(this);
}

FraxSettingsWidget::~FraxSettingsWidget()
{
    delete ui;
}
