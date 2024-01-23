#include "audiometer_settings_widget.h"
#include "ui_audiometer_settings_widget.h"


#include <QSettings>
#include <QDebug>

AudiometerSettingsWidget::AudiometerSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudiometerSettingsWidget)
{
    ui->setupUi(this);
}

AudiometerSettingsWidget::~AudiometerSettingsWidget()
{
    delete ui;
}
