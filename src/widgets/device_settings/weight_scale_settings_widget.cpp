#include "weight_scale_settings_widget.h"
#include "ui_weight_scale_settings_widget.h"

#include "widgets/serial_port_picker_widget.h"

#include <QSettings>


WeightScaleSettingsWidget::WeightScaleSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WeightScaleSettingsWidget)
{
    ui->setupUi(this);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    QSerialPort::BaudRate baudRate = settings.value("weight_scale/baudRate", QSerialPort::BaudRate::Baud9600).value<QSerialPort::BaudRate>();
    QSerialPort::DataBits dataBits = settings.value("weight_scale/dataBits", QSerialPort::DataBits::Data8).value<QSerialPort::DataBits>();
    QSerialPort::Parity parity = settings.value("weight_scale/parity", QSerialPort::Parity::NoParity).value<QSerialPort::Parity>();

    ui->serialPortPicker->setBaudRate(baudRate);
    ui->serialPortPicker->setDataBits(dataBits);
    ui->serialPortPicker->setParity(parity);

    connect(ui->serialPortPicker, &SerialPortPickerWidget::baudRateChanged, this, [&](QSerialPort::BaudRate baudRate) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("weight_scale/baudRate", baudRate);
    });

    connect(ui->serialPortPicker, &SerialPortPickerWidget::dataBitsChanged, this, [&](QSerialPort::DataBits dataBits) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("weight_scale/dataBits", dataBits);
    });

    connect(ui->serialPortPicker, &SerialPortPickerWidget::parityChanged, this, [&](QSerialPort::Parity parity) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("weight_scale/parity", parity);
    });
}

WeightScaleSettingsWidget::~WeightScaleSettingsWidget()
{
    delete ui;
}
