#ifndef TONOMETER_SETTINGS_WIDGET_H
#define TONOMETER_SETTINGS_WIDGET_H

#include <QWidget>

namespace Ui {
class TonometerSettingsWidget;
}

class TonometerSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TonometerSettingsWidget(QWidget *parent = nullptr);
    ~TonometerSettingsWidget();

private:
    Ui::TonometerSettingsWidget *ui;
};

#endif // TONOMETER_SETTINGS_WIDGET_H
