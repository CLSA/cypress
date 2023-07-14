#include "serial_port_widget.h"
#include "ui_serial_port_widget.h"

SerialPortWidget::SerialPortWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPortWidget)
{
    ui->setupUi(this);
    connect(ui->deviceComboBox, &QComboBox::currentTextChanged, this, &SerialPortWidget::comboBoxChanged);
}

SerialPortWidget::~SerialPortWidget()
{
    delete ui;
}

void SerialPortWidget::devicesDiscovered(QMap<QString, QSerialPortInfo> devices)
{
    ui->deviceComboBox->clear();
    for (const auto& device : devices)
    {
        QString label = device.portName();
        ui->deviceComboBox->addItem(label);
    }
}

void SerialPortWidget::comboBoxChanged(QString portNumber)
{
    emit deviceSelected(portNumber);
}
