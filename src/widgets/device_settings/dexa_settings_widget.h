#ifndef DEXA_SETTINGS_WIDGET_H
#define DEXA_SETTINGS_WIDGET_H

#include <QWidget>

namespace Ui {
class DexaSettingsWidget;
}

class DexaSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DexaSettingsWidget(QWidget *parent = nullptr);
    ~DexaSettingsWidget();

private:
    Ui::DexaSettingsWidget *ui;
};

#endif // DEXA_SETTINGS_WIDGET_H
