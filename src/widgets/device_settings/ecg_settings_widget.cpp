#include "ecg_settings_widget.h"
#include "ui_ecg_settings_widget.h"

#include <QSettings>

ECGSettingsWidget::ECGSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ECGSettingsWidget)
{
    ui->setupUi(this);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    QString cardioPath = settings.value("ecg/mac1600/cardioPath", "C:/CARDIO/").toString();
    QString initPath = settings.value("ecg/mac1600/initPath", "C:/CARDIO/initecg/").toString();
    QString databasePath = settings.value("ecg/mac1600/databasePath", "C:/CARDIO/DATABASE/").toString();
    QString exportPath = settings.value("ecg/mac1600/exportPath", "C:/CARDIO/Export/").toString();
    QString settingsFileName = settings.value("ecg/mac1600/settingsFileName", "CARDIO.INI").toString();
    QString winSettingsFileName = settings.value("ecg/mac1600/winSettingsFileName", "WIN.INI").toString();
    QString btrRecordFileName = settings.value("ecg/mac1600/btrRecordFileName", "btr-record.dat").toString();
    QString btrDatabaseFileName = settings.value("ecg/mac1600/btrDatabaseFileName", "PATIENT.BTR").toString();
    QString xmlFileName = settings.value("ecg/mac1600/xmlFileName", "Ecg.xml").toString();
    QString pdfFileNameRestingEcg = settings.value("ecg/mac1600/pdfFileNameRestingEcg", "EcgResting.pdf").toString();
    QString pdfFileNameFullEcg = settings.value("ecg/mac1600/pdfFileNameFullEcg", "EcgFull.pdf").toString();
    QString executableForParticipantInfo = settings.value("ecg/mac1600/executableForParticipantInfo", "butil.exe -LOAD").toString();

    ui->cardioPath->setTitle("Cardio Path");
    ui->cardioPath->setIsDir(true);
    ui->cardioPath->setPath(cardioPath);

    ui->initPath->setTitle("Init Path");
    ui->cardioPath->setIsDir(true);
    ui->initPath->setPath(initPath);

    ui->databasePath->setTitle("Database Path");
    ui->cardioPath->setIsDir(true);
    ui->databasePath->setPath(databasePath);

    ui->exportPath->setTitle("Export Path");
    ui->cardioPath->setIsDir(true);
    ui->exportPath->setPath(exportPath);

    ui->settingsFileName->setTitle("Settings File Name");
    ui->settingsFileName->setPath(settingsFileName);

    ui->winSettingsFileName->setTitle("Win Settings File Name");
    ui->winSettingsFileName->setPath(winSettingsFileName);

    ui->btrRecordFileName->setTitle("Btr Record File Name");
    ui->btrRecordFileName->setPath(btrRecordFileName);

    ui->btrDatabaseFileName->setTitle("Btr Database File Name");
    ui->btrDatabaseFileName->setPath(btrDatabaseFileName);

    ui->xmlFileName->setTitle("XML File Name");
    ui->xmlFileName->setPath(xmlFileName);

    ui->pdfFileNameRestingEcg->setTitle("PDF File Name Resting ECG");
    ui->pdfFileNameRestingEcg->setPath(pdfFileNameRestingEcg);

    ui->pdfFileNameFullEcg->setTitle("PDF File Name Full ECG");
    ui->pdfFileNameFullEcg->setPath(pdfFileNameFullEcg);

    ui->executableForParticipantInfo->setTitle("Executable For Participant Info");
    ui->executableForParticipantInfo->setPath(executableForParticipantInfo);

    connect(ui->cardioPath, &FilePickerWidget::pathChanged, this, [=](QString cardioPath) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("ecg/mac1600/cardioPath", cardioPath);
    });

    connect(ui->initPath, &FilePickerWidget::pathChanged, this, [=](QString initPath) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("ecg/mac1600/initPath", initPath);
    });

    connect(ui->databasePath, &FilePickerWidget::pathChanged, this, [=](QString databasePath) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("ecg/mac1600/databasePath", databasePath);
    });

    connect(ui->exportPath, &FilePickerWidget::pathChanged, this, [=](QString exportPath) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("ecg/mac1600/exportPath", exportPath);
    });

    connect(ui->settingsFileName, &FilePickerWidget::pathChanged, this, [=](QString settingsFileName) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("ecg/mac1600/settingsFileName", settingsFileName);
    });

    connect(ui->winSettingsFileName, &FilePickerWidget::pathChanged, this, [=](QString winSettingsFileName) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("ecg/mac1600/winSettingsFileName", winSettingsFileName);
    });

    connect(ui->btrRecordFileName, &FilePickerWidget::pathChanged, this, [=](QString btrRecordFileName) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("ecg/mac1600/btrRecordFileName", btrRecordFileName);
    });

    connect(ui->btrDatabaseFileName, &FilePickerWidget::pathChanged, this, [=](QString btrDatabaseFileName) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("ecg/mac1600/btrDatabaseFileName", btrDatabaseFileName);
    });

    connect(ui->xmlFileName, &FilePickerWidget::pathChanged, this, [=](QString xmlFileName) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("ecg/mac1600/xmlFileName", xmlFileName);
    });

    connect(ui->pdfFileNameRestingEcg, &FilePickerWidget::pathChanged, this, [=](QString pdfFileNameRestingEcg) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("ecg/mac1600/pdfFileNameRestingEcg", pdfFileNameRestingEcg);
    });

    connect(ui->pdfFileNameFullEcg, &FilePickerWidget::pathChanged, this, [=](QString pdfFileNameFullEcg) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("ecg/mac1600/pdfFileNameFullEcg", pdfFileNameFullEcg);
    });

    connect(ui->executableForParticipantInfo, &FilePickerWidget::pathChanged, this, [=](QString executableForParticipantInfo) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("ecg/mac1600/executableForParticipantInfo", executableForParticipantInfo);
    });
}

ECGSettingsWidget::~ECGSettingsWidget()
{
    delete ui;
}
