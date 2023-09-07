#ifndef BODY_COMPOSITION_SETTINGS_WIDGET_H
#define BODY_COMPOSITION_SETTINGS_WIDGET_H

#include <QWidget>

namespace Ui {
class BodyCompositionSettingsWidget;
}

class BodyCompositionSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BodyCompositionSettingsWidget(QWidget *parent = nullptr);
    ~BodyCompositionSettingsWidget();

private:
    Ui::BodyCompositionSettingsWidget *ui;
};

#endif // BODY_COMPOSITION_SETTINGS_WIDGET_H
