#include "spirometer_settings_widget.h"
#include "ui_spirometer_settings_widget.h"

#include "cypress_settings.h"

SpirometerSettingsWidget::SpirometerSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpirometerSettingsWidget)
{
    ui->setupUi(this);

    QString runnableName = CypressSettings::readSetting("spirometer/runnableName").toString();
    QString runnablePath = CypressSettings::readSetting("spirometer/runnablePath").toString();
    QString dataPath = CypressSettings::readSetting("spirometer/exchangePath").toString();

    ui->runnableName->setTitle("EasyOnPC Executable");
    ui->runnableName->setIsExe(true);
    ui->runnableName->setPath(runnableName);

    ui->runnablePath->setTitle("EasyOnPC Working Directory");
    ui->runnablePath->setIsDir(true);
    ui->runnablePath->setPath(runnablePath);

    ui->dataPath->setTitle("Exchange Folder");
    ui->dataPath->setIsDir(true);
    ui->dataPath->setPath(dataPath);

    connect(ui->runnableName, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("spirometer/runnableName", value);
    });
    connect(ui->runnablePath, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("spirometer/runnablePath", value);
    });
    connect(ui->dataPath, &FilePickerWidget::pathChanged, this, [=](QString value) {
        CypressSettings::writeSetting("spirometer/exchangePath", value);
    });
}

SpirometerSettingsWidget::~SpirometerSettingsWidget()
{
    delete ui;
}
