#include "spirometer_settings_widget.h"
#include "ui_spirometer_settings_widget.h"

SpirometerSettingsWidget::SpirometerSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpirometerSettingsWidget)
{
    ui->setupUi(this);
}

SpirometerSettingsWidget::~SpirometerSettingsWidget()
{
    delete ui;
}
