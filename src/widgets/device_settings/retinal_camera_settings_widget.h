#ifndef RETINAL_CAMERA_SETTINGS_WIDGET_H
#define RETINAL_CAMERA_SETTINGS_WIDGET_H

#include <QWidget>

namespace Ui {
class RetinalCameraSettingsWidget;
}

class RetinalCameraSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RetinalCameraSettingsWidget(QWidget *parent = nullptr);
    ~RetinalCameraSettingsWidget();

private:
    Ui::RetinalCameraSettingsWidget *ui;
};

#endif // RETINAL_CAMERA_SETTINGS_WIDGET_H
