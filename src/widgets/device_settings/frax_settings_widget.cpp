#include "frax_settings_widget.h"
#include "ui_frax_settings_widget.h"

#include "cypress_settings.h"

FraxSettingsWidget::FraxSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FraxSettingsWidget)
{
    ui->setupUi(this);

    QString runnablePath = CypressSettings::readSetting("frax/runnablePath").toString();
    QString runnableName = CypressSettings::readSetting("frax/runnableName").toString();
    QString inputFilePath = CypressSettings::readSetting("frax/inputFilePath").toString();
    QString outputFilePath = CypressSettings::readSetting("frax/outputFilePath").toString();
    QString temporaryFilePath = CypressSettings::readSetting("frax/temporaryFilePath").toString();

    ui->runnableName->setTitle("Executable");
    ui->runnableName->setIsFile(true);
    ui->runnableName->setPath(runnableName);

    ui->runnablePath->setTitle("Working Directory");
    ui->runnablePath->setIsDir(true);
    ui->runnablePath->setPath(runnablePath);

    ui->inputFilePath->setTitle("Input File");
    ui->inputFilePath->setIsFile(true);
    ui->inputFilePath->setPath(inputFilePath);

    ui->outputFilePath->setTitle("Output File");
    ui->outputFilePath->setIsFile(true);
    ui->outputFilePath->setPath(outputFilePath);

    ui->temporaryFilePath->setTitle("Temporary Path");
    ui->temporaryFilePath->setIsDir(true);
    ui->temporaryFilePath->setPath(temporaryFilePath);

    connect(ui->runnableName, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("frax/runnableName", path);
    });

    connect(ui->runnablePath, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("frax/runnablePath", path);
    });

    connect(ui->inputFilePath, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("frax/inputFilePath", path);
    });

    connect(ui->outputFilePath, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("frax/outputFilePath", path);
    });

    connect(ui->temporaryFilePath, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("frax/temporaryFilePath", path);
    });
}

FraxSettingsWidget::~FraxSettingsWidget()
{
    delete ui;
}
