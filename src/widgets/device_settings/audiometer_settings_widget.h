#ifndef AUDIOMETER_SETTINGS_WIDGET_H
#define AUDIOMETER_SETTINGS_WIDGET_H

#include <QWidget>

namespace Ui {
class AudiometerSettingsWidget;
}

class AudiometerSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AudiometerSettingsWidget(QWidget *parent = nullptr);
    ~AudiometerSettingsWidget();

private:
    Ui::AudiometerSettingsWidget *ui;
};

#endif // AUDIOMETER_SETTINGS_WIDGET_H
