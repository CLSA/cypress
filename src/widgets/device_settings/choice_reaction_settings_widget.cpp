#include "choice_reaction_settings_widget.h"
#include "ui_choice_reaction_settings_widget.h"

ChoiceReactionSettingsWidget::ChoiceReactionSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChoiceReactionSettingsWidget)
{
    ui->setupUi(this);
}

ChoiceReactionSettingsWidget::~ChoiceReactionSettingsWidget()
{
    delete ui;
}
