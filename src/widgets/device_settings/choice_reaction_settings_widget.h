#ifndef CHOICE_REACTION_SETTINGS_WIDGET_H
#define CHOICE_REACTION_SETTINGS_WIDGET_H

#include <QWidget>

namespace Ui {
class ChoiceReactionSettingsWidget;
}

class ChoiceReactionSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChoiceReactionSettingsWidget(QWidget *parent = nullptr);
    ~ChoiceReactionSettingsWidget();

private:
    Ui::ChoiceReactionSettingsWidget *ui;
};

#endif // CHOICE_REACTION_SETTINGS_WIDGET_H
