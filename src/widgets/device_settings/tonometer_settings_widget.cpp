#include "tonometer_settings_widget.h"
#include "ui_tonometer_settings_widget.h"

#include "cypress_settings.h"

TonometerSettingsWidget::TonometerSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TonometerSettingsWidget)
{
    ui->setupUi(this);

    QString runnablePath = CypressSettings::readSetting("tonometer/runnablePath").toString();
    QString databasePath = CypressSettings::readSetting("tonometer/databasePath").toString();
    QString temporaryPath = CypressSettings::readSetting("tonometer/temporaryPath").toString();

    ui->runnablePath->setTitle("Executable");
    ui->runnablePath->setIsFile(true);
    ui->runnablePath->setPath(runnablePath);

    ui->databasePath->setTitle("Database (.mdb)");
    ui->databasePath->setIsDir(false);
    ui->databasePath->setPath(runnablePath);

    ui->temporaryPath->setTitle("Backup Path");
    ui->temporaryPath->setIsDir(true);
    ui->temporaryPath->setPath(temporaryPath);

    connect(ui->runnablePath, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("tonometer/runnablePath", path);
    });

    connect(ui->databasePath, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("tonometer/databasePath", path);
    });

    connect(ui->temporaryPath, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("tonometer/temporaryPath", path);
    });
}

TonometerSettingsWidget::~TonometerSettingsWidget()
{
    delete ui;
}
