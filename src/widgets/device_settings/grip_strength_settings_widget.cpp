#include "grip_strength_settings_widget.h"
#include "ui_grip_strength_settings_widget.h"

GripStrengthSettingsWidget::GripStrengthSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GripStrengthSettingsWidget)
{
    ui->setupUi(this);
}

GripStrengthSettingsWidget::~GripStrengthSettingsWidget()
{
    delete ui;
}
