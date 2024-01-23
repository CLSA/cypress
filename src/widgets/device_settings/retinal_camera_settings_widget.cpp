#include "retinal_camera_settings_widget.h"
#include "ui_retinal_camera_settings_widget.h"

#include "cypress_settings.h"

RetinalCameraSettingsWidget::RetinalCameraSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RetinalCameraSettingsWidget)
{
    ui->setupUi(this);

    QString runnablePath = CypressSettings::readSetting("retinal_camera/runnablePath").toString();
    QString runnableName = CypressSettings::readSetting("retinal_camera/runnableName").toString();

    ui->runnableName->setTitle("Executable");
    ui->runnableName->setIsFile(true);
    ui->runnableName->setPath(runnableName);

    ui->runnablePath->setTitle("Working Directory");
    ui->runnablePath->setIsDir(true);
    ui->runnablePath->setPath(runnablePath);

    connect(ui->runnableName, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("retinal_camera/runnableName", path);
    });

    connect(ui->runnablePath, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("retinal_camera/runnablePath", path);
    });
}

RetinalCameraSettingsWidget::~RetinalCameraSettingsWidget()
{
    delete ui;
}
