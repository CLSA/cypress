#include "usb_port_picker.h"
#include "ui_usb_port_picker.h"

UsbPortPicker::UsbPortPicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsbPortPicker)
{
    ui->setupUi(this);

    ui->usbPortComboBox->setEnabled(false);
    ui->usbConnectPushButton->setEnabled(false);
    ui->usbDisconnectPushButton->setEnabled(false);

    connect(ui->usbPortComboBox, &QComboBox::currentTextChanged, this, &UsbPortPicker::comboBoxChanged);
    connect(ui->usbConnectPushButton, &QPushButton::pressed, this, [this]() {
        emit connectDevice();
    });

    connect(ui->usbDisconnectPushButton, &QPushButton::pressed, this, [this]() {
        emit disconnectDevice();
    });
}

UsbPortPicker::~UsbPortPicker()
{
    delete ui;
}

void UsbPortPicker::comboBoxChanged(const QString& portName)
{
    ui->usbPortComboBox->blockSignals(true);
    foreach (auto device, m_usbPorts)
    {
        if (device == portName)
        {
            ui->usbPortComboBox->setCurrentIndex(ui->usbPortComboBox->findText(portName));

            ui->usbConnectPushButton->setEnabled(false);
            ui->usbDisconnectPushButton->setEnabled(false);

            emit deviceSelected(device);

            break;
        }
    }
    ui->usbPortComboBox->blockSignals(false);
}

void UsbPortPicker::scanningForDevices()
{
    clear();

    ui->usbPortComboBox->setEnabled(false);
    ui->usbConnectPushButton->setEnabled(false);
    ui->usbDisconnectPushButton->setEnabled(false);
}

void UsbPortPicker::devicesCanBeSelected()
{
    ui->usbPortComboBox->setEnabled(true);
    ui->usbConnectPushButton->setEnabled(false);
    ui->usbDisconnectPushButton->setEnabled(false);
}

void UsbPortPicker::defaultDeviceSelected(const QUsb::Id &portInfo)
{
    comboBoxChanged(portInfo);
}

void UsbPortPicker::deviceConnected()
{
    ui->usbConnectPushButton->setEnabled(false);
    ui->usbDisconnectPushButton->setEnabled(true);
    ui->usbPortComboBox->setEnabled(false);
}

void UsbPortPicker::deviceDisconnected()
{
    ui->usbConnectPushButton->setEnabled(true);
    ui->usbDisconnectPushButton->setEnabled(false);
    ui->usbPortComboBox->setEnabled(true);
}

void UsbPortPicker::devicesDiscovered(const QMap<QString, QUsb::Id> &devices)
{
    clear();
    ui->usbPortComboBox->blockSignals(true);

    for (const auto& device : devices)
    {
        ui->usbPortComboBox->addItem(device);
    }

    ui->usbPortComboBox->blockSignals(false);
    ui->usbPortComboBox->setEnabled(true);

    m_usbPorts = devices;
}

void UsbPortPicker::canConnectDevice()
{
    ui->usbConnectPushButton->setEnabled(true);
    ui->usbDisconnectPushButton->setEnabled(false);
    ui->usbPortComboBox->setEnabled(true);
}

void UsbPortPicker::canDisconnectDevice()
{
    ui->usbConnectPushButton->setEnabled(false);
    ui->usbDisconnectPushButton->setEnabled(false);
    ui->usbPortComboBox->setEnabled(false);
}

void UsbPortPicker::clear()
{
    ui->usbPortComboBox->clear();
}
