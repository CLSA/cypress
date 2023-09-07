#include "ecg_settings_widget.h"
#include "ui_ecg_settings_widget.h"

ECGSettingsWidget::ECGSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ECGSettingsWidget)
{
    ui->setupUi(this);
}

ECGSettingsWidget::~ECGSettingsWidget()
{
    delete ui;
}
