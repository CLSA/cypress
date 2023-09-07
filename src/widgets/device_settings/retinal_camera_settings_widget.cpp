#include "retinal_camera_settings_widget.h"
#include "ui_retinal_camera_settings_widget.h"

RetinalCameraSettingsWidget::RetinalCameraSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RetinalCameraSettingsWidget)
{
    ui->setupUi(this);
}

RetinalCameraSettingsWidget::~RetinalCameraSettingsWidget()
{
    delete ui;
}
