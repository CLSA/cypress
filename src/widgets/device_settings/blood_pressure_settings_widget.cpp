#include "blood_pressure_settings_widget.h"
#include "ui_blood_pressure_settings_widget.h"

#include <QSettings>

BloodPressureSettingsWidget::BloodPressureSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BloodPressureSettingsWidget)
{
    ui->setupUi(this);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    QSerialPort::BaudRate baudRate = settings.value("bpm/baudRate", QSerialPort::BaudRate::Baud9600).value<QSerialPort::BaudRate>();
    QSerialPort::DataBits dataBits = settings.value("bpm/dataBits", QSerialPort::DataBits::Data8).value<QSerialPort::DataBits>();
    QSerialPort::Parity parity = settings.value("bpm/parity", QSerialPort::Parity::NoParity).value<QSerialPort::Parity>();

    ui->serialPortPicker->setBaudRate(baudRate);
    ui->serialPortPicker->setDataBits(dataBits);
    ui->serialPortPicker->setParity(parity);

    connect(ui->serialPortPicker, &SerialPortPickerWidget::baudRateChanged, this, [&](QSerialPort::BaudRate baudRate) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("bpm/baudRate", baudRate);
    });

    connect(ui->serialPortPicker, &SerialPortPickerWidget::dataBitsChanged, this, [&](QSerialPort::DataBits dataBits) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("bpm/dataBits", dataBits);
    });

    connect(ui->serialPortPicker, &SerialPortPickerWidget::parityChanged, this, [&](QSerialPort::Parity parity) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
        settings.setValue("bpm/parity", parity);
    });
}

BloodPressureSettingsWidget::~BloodPressureSettingsWidget()
{
    delete ui;
}
