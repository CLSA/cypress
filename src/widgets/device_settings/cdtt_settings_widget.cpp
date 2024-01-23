#include "cdtt_settings_widget.h"
#include "ui_cdtt_settings_widget.h"

#include "cypress_settings.h"

#include <QDebug>

CDTTSettingsWidget::CDTTSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDTTSettingsWidget)
{
    ui->setupUi(this);

    QString runnablePath = CypressSettings::readSetting("cdtt/runnablePath").toString();
    QString runnableName = CypressSettings::readSetting("cdtt/runnableName").toString();
    QString outputPath = CypressSettings::readSetting("cdtt/outputPath").toString();

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
        CypressSettings::writeSetting("cdtt/runnableName", path);
    });

    connect(ui->runnablePath, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("cdtt/runnablePath", path);
    });

    connect(ui->outputPath, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("cdtt/outputPath", path);
    });
}

CDTTSettingsWidget::~CDTTSettingsWidget()
{
    delete ui;
}
