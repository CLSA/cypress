#include "tonometer_settings_widget.h"
#include "ui_tonometer_settings_widget.h"

TonometerSettingsWidget::TonometerSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TonometerSettingsWidget)
{
    ui->setupUi(this);
}

TonometerSettingsWidget::~TonometerSettingsWidget()
{
    delete ui;
}
