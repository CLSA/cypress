#ifndef CDTT_SETTINGS_WIDGET_H
#define CDTT_SETTINGS_WIDGET_H

#include <QWidget>

namespace Ui {
class CDTTSettingsWidget;
}

class CDTTSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CDTTSettingsWidget(QWidget *parent = nullptr);
    ~CDTTSettingsWidget();

private:
    Ui::CDTTSettingsWidget *ui;
};

#endif // CDTT_SETTINGS_WIDGET_H
