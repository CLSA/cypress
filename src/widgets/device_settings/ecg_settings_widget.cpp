#include "ecg_settings_widget.h"
#include "ui_ecg_settings_widget.h"

#include <QSettings>

ECGSettingsWidget::ECGSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ECGSettingsWidget)
{
    ui->setupUi(this);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    QString runnableName = settings.value("ecg/runnableName", "C:/CARDIO/").toString();
    QString workingPath = settings.value("ecg/workingPath", "C:/CARDIO/").toString();
    QString exportPath = settings.value("ecg/exportPath", "C:/CARDIO/Export/").toString();

    ui->runnableName->setTitle("CardioSoft exe");
    ui->runnableName->setIsFile(true);
    ui->runnableName->setPath(runnableName);

    ui->workingPath->setTitle("Working Path");
    ui->workingPath->setIsDir(true);
    ui->workingPath->setPath(workingPath);

    ui->exportPath->setTitle("Export Path");
    ui->exportPath->setIsDir(true);
    ui->exportPath->setPath(exportPath);

    connect(ui->runnableName, &FilePickerWidget::pathChanged, this, [=](QString cardioPath) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("ecg/mac1600/cardioPath", cardioPath);
    });

    connect(ui->workingPath, &FilePickerWidget::pathChanged, this, [=](QString initPath) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("ecg/mac1600/initPath", initPath);
    });

    connect(ui->exportPath, &FilePickerWidget::pathChanged, this, [=](QString databasePath) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("ecg/mac1600/databasePath", databasePath);
    });
}

ECGSettingsWidget::~ECGSettingsWidget()
{
    delete ui;
}
