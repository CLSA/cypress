#include "blood_pressure_settings_widget.h"
#include "ui_blood_pressure_settings_widget.h"

#include <QSettings>

BloodPressureSettingsWidget::BloodPressureSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BloodPressureSettingsWidget)
{
    ui->setupUi(this);
}

BloodPressureSettingsWidget::~BloodPressureSettingsWidget()
{
    delete ui;
}
