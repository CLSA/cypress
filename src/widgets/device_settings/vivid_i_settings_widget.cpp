#include "vivid_i_settings_widget.h"
#include "ui_vivid_i_settings_widget.h"

#include "cypress_settings.h"

VividISettingsWidget::VividISettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VividISettingsWidget)
{
    ui->setupUi(this);

    QString runnableName = CypressSettings::readSetting("ultrasound/dicom/runnableName").toString();
    QString runnablePath = CypressSettings::readSetting("ultrasound/dicom/runnablePath").toString();

    QString storageDirPath = CypressSettings::readSetting("ultrasound/dicom/storagePath").toString();
    QString logConfigPath = CypressSettings::readSetting("ultrasound/dicom/log_config").toString();
    QString ascConfigPath = CypressSettings::readSetting("ultrasound/dicom/asc_config").toString();

    ui->runnableName->setTitle("DICOM Executable Path");
    ui->runnableName->setIsExe(true);
    ui->runnableName->setPath(runnableName);

    ui->runnablePath->setTitle("DICOM Working Directory Path");
    ui->runnablePath->setIsDir(true);
    ui->runnablePath->setPath(runnablePath);

    ui->storagePath->setTitle("DICOM Storage Path");
    ui->storagePath->setIsDir(true);
    ui->storagePath->setPath(storageDirPath);

    ui->logConfigPath->setTitle("DICOM Log Config File Path");
    ui->logConfigPath->setIsFile(true);
    ui->logConfigPath->setPath(logConfigPath);

    ui->ascConfigPath->setTitle("DICOM Settings File Path");
    ui->ascConfigPath->setIsFile(true);
    ui->ascConfigPath->setPath(ascConfigPath);

    connect(ui->runnableName, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("ultrasound/dicom/runnableName", value);
    });
    connect(ui->runnablePath, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("ultrasound/dicom/runnablePath", value);
    });
    connect(ui->storagePath, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("ultrasound/dicom/storagePath", value);
    });
    connect(ui->logConfigPath, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("ultrasound/dicom/log_config", value);
    });
    connect(ui->ascConfigPath, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("ultrasound/dicom/asc_config", value);
    });
}

VividISettingsWidget::~VividISettingsWidget()
{
    delete ui;
}
