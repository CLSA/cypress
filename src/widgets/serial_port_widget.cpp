#include "serial_port_widget.h"
#include "ui_serial_port_widget.h"

#include <QDebug>

SerialPortWidget::SerialPortWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPortWidget)
{
    ui->setupUi(this);

    ui->deviceComboBox->setEnabled(false);
    ui->connectButton->setEnabled(false);
    ui->disconnectButton->setEnabled(false);

    connect(ui->deviceComboBox, &QComboBox::currentTextChanged, this, &SerialPortWidget::comboBoxChanged);
    connect(ui->connectButton, &QPushButton::pressed, this, [this]() {
        emit connectDevice();
    });

    connect(ui->disconnectButton, &QPushButton::pressed, this, [this]() {
        emit disconnectDevice();
    });
}

SerialPortWidget::~SerialPortWidget()
{
    delete ui;
}

void SerialPortWidget::scanningForDevices()
{
    clear();

    ui->deviceComboBox->setEnabled(false);
    ui->connectButton->setEnabled(false);
    ui->disconnectButton->setEnabled(false);
}

void SerialPortWidget::deviceFound(const QSerialPortInfo& portInfo) {
    ui->deviceComboBox->blockSignals(true);
    ui->deviceComboBox->setCurrentIndex(ui->deviceComboBox->findText(portInfo.portName()));
    ui->deviceComboBox->blockSignals(false);

    m_portInfo = portInfo;
}

void SerialPortWidget::devicesCanBeSelected()
{
    ui->deviceComboBox->setEnabled(true);
    ui->connectButton->setEnabled(false);
    ui->disconnectButton->setEnabled(false);
}

void SerialPortWidget::defaultDeviceSelected(const QSerialPortInfo &portInfo)
{
    comboBoxChanged(portInfo.portName());

    m_portInfo = portInfo;
}

void SerialPortWidget::deviceConnected(const QSerialPortInfo &portInfo)
{
    ui->connectButton->setEnabled(false);
    ui->disconnectButton->setEnabled(true);
    ui->deviceComboBox->setEnabled(false);

    m_portInfo = portInfo;
}

void SerialPortWidget::deviceDisconnected()
{
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->deviceComboBox->setEnabled(true);
}

void SerialPortWidget::devicesDiscovered(const QMap<QString, QSerialPortInfo> &devices)
{
    clear();
    ui->deviceComboBox->blockSignals(true);

    for (const auto& device : devices)
    {
        QString label = device.portName();
        ui->deviceComboBox->addItem(label);
    }

    ui->deviceComboBox->blockSignals(false);

    ui->deviceComboBox->setEnabled(true);

    m_serialPorts = devices;
}

void SerialPortWidget::canConnectDevice()
{
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->deviceComboBox->setEnabled(true);
}

void SerialPortWidget::canDisconnectDevice()
{
    ui->connectButton->setEnabled(false);
    ui->disconnectButton->setEnabled(false);
    ui->deviceComboBox->setEnabled(false);
}

void SerialPortWidget::clear()
{
    ui->deviceComboBox->clear();
}

void SerialPortWidget::comboBoxChanged(const QString& portName)
{
    ui->deviceComboBox->blockSignals(true);
    foreach (auto device, m_serialPorts)
    {
        if (device.portName() == portName)
        {
            ui->deviceComboBox->setCurrentIndex(ui->deviceComboBox->findText(portName));
            ui->connectButton->setEnabled(false);
            ui->disconnectButton->setEnabled(false);

            m_portInfo = device;

            qDebug() << device.manufacturer();
            qDebug() << device.productIdentifier();
            qDebug() << device.description();

            emit deviceSelected(device);

            break;
        }
    }
    ui->deviceComboBox->blockSignals(false);
}
