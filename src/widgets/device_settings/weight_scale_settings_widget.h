#ifndef WEIGHT_SCALE_SETTINGS_WIDGET_H
#define WEIGHT_SCALE_SETTINGS_WIDGET_H

#include <QWidget>

namespace Ui {
class WeightScaleSettingsWidget;
}

class WeightScaleSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WeightScaleSettingsWidget(QWidget *parent = nullptr);
    ~WeightScaleSettingsWidget();

private:
    Ui::WeightScaleSettingsWidget *ui;
};

#endif // WEIGHT_SCALE_SETTINGS_WIDGET_H
