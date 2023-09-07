#ifndef SIGNATURE_PAD_SETTINGS_WIDGET_H
#define SIGNATURE_PAD_SETTINGS_WIDGET_H

#include <QWidget>

namespace Ui {
class SignaturePadSettingsWidget;
}

class SignaturePadSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SignaturePadSettingsWidget(QWidget *parent = nullptr);
    ~SignaturePadSettingsWidget();

private:
    Ui::SignaturePadSettingsWidget *ui;
};

#endif // SIGNATURE_PAD_SETTINGS_WIDGET_H
