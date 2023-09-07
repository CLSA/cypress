#include "cdtt_settings_widget.h"
#include "ui_cdtt_settings_widget.h"

CDTTSettingsWidget::CDTTSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDTTSettingsWidget)
{
    ui->setupUi(this);
}

CDTTSettingsWidget::~CDTTSettingsWidget()
{
    delete ui;
}
