#ifndef BLOOD_PRESSURE_SETTINGS_WIDGET_H
#define BLOOD_PRESSURE_SETTINGS_WIDGET_H

#include <QWidget>

namespace Ui {
class BloodPressureSettingsWidget;
}

class BloodPressureSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BloodPressureSettingsWidget(QWidget *parent = nullptr);
    ~BloodPressureSettingsWidget();

private:
    Ui::BloodPressureSettingsWidget *ui;
};

#endif // BLOOD_PRESSURE_SETTINGS_WIDGET_H
