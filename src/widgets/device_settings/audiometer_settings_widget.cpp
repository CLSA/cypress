#include "audiometer_settings_widget.h"
#include "ui_audiometer_settings_widget.h"

#include "widgets/serial_port_picker_widget.h"

#include <QSettings>
#include <QDebug>

AudiometerSettingsWidget::AudiometerSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudiometerSettingsWidget)
{
    ui->setupUi(this);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    QSerialPort::BaudRate baudRate = settings.value("audiometer/ra300/baudRate", QSerialPort::BaudRate::Baud9600).value<QSerialPort::BaudRate>();
    QSerialPort::DataBits dataBits = settings.value("audiometer/ra300/dataBits", QSerialPort::DataBits::Data8).value<QSerialPort::DataBits>();
    QSerialPort::Parity parity = settings.value("audiometer/ra300/parity", QSerialPort::Parity::NoParity).value<QSerialPort::Parity>();

    ui->serialPortPicker->setBaudRate(baudRate);
    ui->serialPortPicker->setDataBits(dataBits);
    ui->serialPortPicker->setParity(parity);

    connect(ui->serialPortPicker, &SerialPortPickerWidget::baudRateChanged, this, [&](QSerialPort::BaudRate baudRate) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("audiometer/ra300/baudRate", baudRate);
    });

    connect(ui->serialPortPicker, &SerialPortPickerWidget::dataBitsChanged, this, [&](QSerialPort::DataBits dataBits) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("audiometer/ra300/dataBits", dataBits);
    });

    connect(ui->serialPortPicker, &SerialPortPickerWidget::parityChanged, this, [&](QSerialPort::Parity parity) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("audiometer/ra300/parity", parity);
    });
}

AudiometerSettingsWidget::~AudiometerSettingsWidget()
{
    delete ui;
}
