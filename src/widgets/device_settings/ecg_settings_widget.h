#ifndef ECG_SETTINGS_WIDGET_H
#define ECG_SETTINGS_WIDGET_H

#include <QWidget>

namespace Ui {
class ECGSettingsWidget;
}

class ECGSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ECGSettingsWidget(QWidget *parent = nullptr);
    ~ECGSettingsWidget();

private:
    Ui::ECGSettingsWidget *ui;
};

#endif // ECG_SETTINGS_WIDGET_H
