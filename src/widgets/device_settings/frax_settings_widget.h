#ifndef FRAX_SETTINGS_WIDGET_H
#define FRAX_SETTINGS_WIDGET_H

#include <QWidget>

namespace Ui {
class FraxSettingsWidget;
}

class FraxSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FraxSettingsWidget(QWidget *parent = nullptr);
    ~FraxSettingsWidget();

private:
    Ui::FraxSettingsWidget *ui;
};

#endif // FRAX_SETTINGS_WIDGET_H
