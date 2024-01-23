#include "choice_reaction_settings_widget.h"
#include "ui_choice_reaction_settings_widget.h"

#include "cypress_settings.h"

#include <QDebug>

ChoiceReactionSettingsWidget::ChoiceReactionSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChoiceReactionSettingsWidget)
{
    ui->setupUi(this);

    QString runnablePath = CypressSettings::readSetting("choice_reaction/runnablePath").toString();
    QString runnableName = CypressSettings::readSetting("choice_reaction/runnableName").toString();
    QString outputPath = CypressSettings::readSetting("choice_reaction/outputPath").toString();

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
        CypressSettings::writeSetting("choice_reaction/runnableName", path);
    });

    connect(ui->runnablePath, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("choice_reaction/runnablePath", path);
    });

    connect(ui->outputPath, &FilePickerWidget::pathChanged, this, [=](QString path) {
        CypressSettings::writeSetting("choice_reaction/outputPath", path);
    });
}

ChoiceReactionSettingsWidget::~ChoiceReactionSettingsWidget()
{
    delete ui;
}
