#include "dexa_settings_widget.h"
#include "cypress_settings.h"

#include "ui_dexa_settings_widget.h"

#include <QSettings>

DexaSettingsWidget::DexaSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DexaSettingsWidget)
{
    ui->setupUi(this);

    QString runnableName = CypressSettings::readSetting("dxa/dicom/runnableName").toString();
    QString runnablePath = CypressSettings::readSetting("dxa/dicom/runnablePath").toString();

    QString storageDirPath = CypressSettings::readSetting("dxa/dicom/storagePath").toString();
    QString logConfigPath = CypressSettings::readSetting("dxa/dicom/log_config").toString();
    QString ascConfigPath = CypressSettings::readSetting("dxa/dicom/asc_config").toString();

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
        CypressSettings::writeSetting("dxa/dicom/runnableName", value);
    });
    connect(ui->runnablePath, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("dxa/dicom/runnablePath", value);
    });
    connect(ui->storagePath, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("dxa/dicom/storagePath", value);
    });
    connect(ui->logConfigPath, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("dxa/dicom/log_config", value);
    });
    connect(ui->ascConfigPath, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("dxa/dicom/asc_config", value);
    });
}

DexaSettingsWidget::~DexaSettingsWidget()
{
    delete ui;
}
