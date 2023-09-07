#ifndef SPIROMETER_SETTINGS_WIDGET_H
#define SPIROMETER_SETTINGS_WIDGET_H

#include <QWidget>

namespace Ui {
class SpirometerSettingsWidget;
}

class SpirometerSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpirometerSettingsWidget(QWidget *parent = nullptr);
    ~SpirometerSettingsWidget();

private:
    Ui::SpirometerSettingsWidget *ui;
};

#endif // SPIROMETER_SETTINGS_WIDGET_H
