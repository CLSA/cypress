#include "grip_strength_settings_widget.h"
#include "ui_grip_strength_settings_widget.h"

#include "cypress_settings.h"

GripStrengthSettingsWidget::GripStrengthSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GripStrengthSettingsWidget)
{
    ui->setupUi(this);

    QString runnableName = CypressSettings::readSetting("grip_strength/runnableName").toString();
    QString runnablePath = CypressSettings::readSetting("grip_strength/runnablePath").toString();

    QString databasePath = CypressSettings::readSetting("grip_strength/databasePath").toString();
    QString backupPath = CypressSettings::readSetting("grip_strength/backupPath").toString();

    QString gripTestDBPath = CypressSettings::readSetting("grip_strength/gripTestDB").toString();
    QString gripTestDataDBPath = CypressSettings::readSetting("grip_strength/gripTestDataDB").toString();

    ui->runnableName->setTitle("Executable");
    ui->runnableName->setIsFile(true);
    ui->runnableName->setPath(runnableName);

    ui->runnablePath->setTitle("Working Directory");
    ui->runnablePath->setIsDir(true);
    ui->runnablePath->setPath(runnablePath);

    ui->databasePath->setTitle("Data directory");
    ui->databasePath->setIsDir(true);
    ui->databasePath->setPath(databasePath);

    ui->backupPath->setTitle("Backup Directory");
    ui->backupPath->setIsDir(true);
    ui->backupPath->setPath(backupPath);

    ui->gripTestDB->setTitle("Grip Test DB");
    ui->gripTestDB->setIsFile(true);
    ui->gripTestDB->setPath(gripTestDBPath);

    ui->gripTestDataDB->setTitle("Grip Test Data DB");
    ui->gripTestDataDB->setIsFile(true);
    ui->gripTestDataDB->setPath(gripTestDataDBPath);

    connect(ui->runnableName, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("grip_strength/runnableName", value);
    });
    connect(ui->runnablePath, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("grip_strength/runnablePath", value);
    });
    connect(ui->databasePath, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("grip_strength/databasePath", value);
    });
    connect(ui->backupPath, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("grip_strength/backupPath", value);
    });
    connect(ui->gripTestDB, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("grip_strength/gripTestDB", value);
    });
    connect(ui->gripTestDataDB, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("grip_strength/gripTestDataDB", value);
    });
}

GripStrengthSettingsWidget::~GripStrengthSettingsWidget()
{
    delete ui;
}
