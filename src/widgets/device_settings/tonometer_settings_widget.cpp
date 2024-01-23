#include "tonometer_settings_widget.h"
#include "ui_tonometer_settings_widget.h"

#include "cypress_settings.h"

TonometerSettingsWidget::TonometerSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TonometerSettingsWidget)
{
    ui->setupUi(this);

    QString runnablePath = CypressSettings::readSetting("tonometer/runnablePath").toString();
    QString runnableName = CypressSettings::readSetting("tonometer/runnableName").toString();
    QString outputPath = CypressSettings::readSetting("tonometer/outputPath").toString();

    ui->runnableName->setTitle("Executable");
    ui->runnableName->setIsFile(true);
    ui->runnableName->setPath(runnableName);

    ui->runnablePath->setTitle("Working Directory");
    ui->runnablePath->setIsDir(true);
    ui->runnablePath->setPath(runnablePath);

    ui->outputPath->setTitle("Output Path");
    ui->outputPath->setIsDir(true);
    ui->outputPath->setPath(outputPath);

    connect(ui->runnableName, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("tonometer/runnableName", path);
    });

    connect(ui->runnablePath, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("tonometer/runnablePath", path);
    });

    connect(ui->outputPath, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("tonometer/outputPath", path);
    });
}

TonometerSettingsWidget::~TonometerSettingsWidget()
{
    delete ui;
}
