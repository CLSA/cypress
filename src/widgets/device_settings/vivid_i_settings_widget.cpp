#include "vivid_i_settings_widget.h"
#include "ui_vivid_i_settings_widget.h"

#include <QSettings>

VividISettingsWidget::VividISettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VividISettingsWidget)
{
    ui->setupUi(this);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    QString aeTitle = settings.value("vividi/dicom/aeTitle", "").toString();
    QString port = settings.value("vividi/dicom/port", "").toString();
    QString exe = settings.value("vividi/dicom/executable", "").toString();
    QString config = settings.value("vividi/dicom/asc_config", "").toString();

    ui->dicomWidget->setDicomLabels(aeTitle, "", port);

    ui->executable->setTitle("Executable");
    ui->executable->setIsExe(true);
    ui->executable->setPath(exe);

    ui->config->setTitle("Config");
    ui->config->setIsFile(true);
    ui->config->setPath(config);

    connect(ui->dicomWidget, &DicomWidget::aeTitleChanged, this, [=](QString aeTitle) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("vividi/dicom/aeTitle", aeTitle);
    });

    connect(ui->dicomWidget, &DicomWidget::portChanged, this, [=](QString port) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("vividi/dicom/port", port);
    });

    connect(ui->executable, &FilePickerWidget::pathChanged, this, [=](QString path) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("vividi/dicom/executable", path);
    });

    connect(ui->config, &FilePickerWidget::pathChanged, this, [=](QString path) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("vividi/dicom/asc_config", path);
    });
}

VividISettingsWidget::~VividISettingsWidget()
{
    delete ui;
}
