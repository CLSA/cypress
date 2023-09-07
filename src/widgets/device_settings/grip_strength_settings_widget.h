#ifndef GRIP_STRENGTH_SETTINGS_WIDGET_H
#define GRIP_STRENGTH_SETTINGS_WIDGET_H

#include <QWidget>

namespace Ui {
class GripStrengthSettingsWidget;
}

class GripStrengthSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GripStrengthSettingsWidget(QWidget *parent = nullptr);
    ~GripStrengthSettingsWidget();

private:
    Ui::GripStrengthSettingsWidget *ui;
};

#endif // GRIP_STRENGTH_SETTINGS_WIDGET_H
