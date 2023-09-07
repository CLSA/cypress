#include "dexa_settings_widget.h"
#include "ui_dexa_settings_widget.h"

#include <QSettings>

DexaSettingsWidget::DexaSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DexaSettingsWidget)
{
    ui->setupUi(this);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    QString aeTitle = settings.value("dxa/dicom/aeTitle", "").toString();
    QString port = settings.value("dxa/dicom/port", "").toString();
    QString exe = settings.value("dxa/dicom/executable", "").toString();
    QString config = settings.value("dxa/dicom/asc_config", "").toString();

    ui->dicomWidget->setDicomLabels(aeTitle, "", port);

    ui->executable->setTitle("Executable");
    ui->executable->setIsExe(true);
    ui->executable->setPath(exe);

    ui->config->setTitle("Config");
    ui->config->setIsFile(true);
    ui->config->setPath(config);

    connect(ui->dicomWidget, &DicomWidget::aeTitleChanged, this, [=](QString aeTitle) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("dxa/dicom/aeTitle", aeTitle);
    });

    connect(ui->dicomWidget, &DicomWidget::portChanged, this, [=](QString port) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("dxa/dicom/port", port);
    });

    connect(ui->executable, &FilePickerWidget::pathChanged, this, [=](QString path) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("dxa/dicom/executable", path);
    });

    connect(ui->config, &FilePickerWidget::pathChanged, this, [=](QString path) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("dxa/dicom/asc_config", path);
    });
}

DexaSettingsWidget::~DexaSettingsWidget()
{
    delete ui;
}
