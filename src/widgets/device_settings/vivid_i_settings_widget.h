#ifndef VIVID_I_SETTINGS_WIDGET_H
#define VIVID_I_SETTINGS_WIDGET_H

#include <QWidget>

namespace Ui {
class VividISettingsWidget;
}

class VividISettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VividISettingsWidget(QWidget *parent = nullptr);
    ~VividISettingsWidget();

private:
    Ui::VividISettingsWidget *ui;
};

#endif // VIVID_I_SETTINGS_WIDGET_H
