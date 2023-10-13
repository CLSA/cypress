#include "serial_port_picker_widget.h"
#include "ui_serial_port_picker_widget.h"

#include <QDebug>
#include <QMessageBox>
#include <QException>

SerialPortPickerWidget::SerialPortPickerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPortPickerWidget)
{
    ui->setupUi(this);

    ui->baudRates->addItem("1200", QSerialPort::BaudRate::Baud1200);
    ui->baudRates->addItem("2400", QSerialPort::BaudRate::Baud2400);
    ui->baudRates->addItem("4800", QSerialPort::BaudRate::Baud4800);
    ui->baudRates->addItem("9600", QSerialPort::BaudRate::Baud9600);
    ui->baudRates->addItem("19200", QSerialPort::BaudRate::Baud19200);
    ui->baudRates->addItem("38400", QSerialPort::BaudRate::Baud38400);
    ui->baudRates->addItem("57600", QSerialPort::BaudRate::Baud57600);
    ui->baudRates->addItem("115200", QSerialPort::BaudRate::Baud115200);

    ui->dataBits->addItem("Data5", QSerialPort::DataBits::Data5);
    ui->dataBits->addItem("Data6", QSerialPort::DataBits::Data6);
    ui->dataBits->addItem("Data7", QSerialPort::DataBits::Data7);
    ui->dataBits->addItem("Data8", QSerialPort::DataBits::Data8);

    ui->parity->addItem("NoParity", QSerialPort::Parity::NoParity);
    ui->parity->addItem("EvenParity", QSerialPort::Parity::EvenParity);
    ui->parity->addItem("OddParity", QSerialPort::Parity::OddParity);
    ui->parity->addItem("SpaceParity", 	QSerialPort::Parity::SpaceParity);
    ui->parity->addItem("MarkParity", QSerialPort::Parity::MarkParity);

    connect(ui->scanDevicesButton, &QPushButton::clicked, this, [=]{
        scanDevices();
    });

    connect(ui->serialPorts, &QComboBox::currentTextChanged, this, [=](const QString& label){
        selectDevice(label);
    });

    connect(ui->baudRates, &QComboBox::currentTextChanged, this, [=]() {
        emit baudRateChanged(ui->baudRates->currentData().value<QSerialPort::BaudRate>());
    });

    connect(ui->dataBits, &QComboBox::currentTextChanged, this, [=]() {
        emit dataBitsChanged(ui->dataBits->currentData().value<QSerialPort::DataBits>());
    });

    connect(ui->parity, &QComboBox::currentTextChanged, this, [=]() {
        emit parityChanged(ui->parity->currentData().value<QSerialPort::Parity>());
    });
}

SerialPortPickerWidget::~SerialPortPickerWidget()
{
    delete ui;
}

const QSerialPortInfo& SerialPortPickerWidget::selectDevice(const QString& label)
{
    if (m_deviceList.contains(label))
    {
        const auto it = m_deviceList.constFind(label);
        if (!it->isNull() && it != m_deviceList.constEnd())
        {
            setDevice(*it);
            return *it;
        }
    }

    throw QException();
}

void SerialPortPickerWidget::setBaudRate(QSerialPort::BaudRate baudRate)
{
    ui->baudRates->blockSignals(true);
    int index = ui->baudRates->findData(baudRate);
    if (index)
    {
        ui->baudRates->setCurrentIndex(index);
    }
    ui->baudRates->blockSignals(false);
}

void SerialPortPickerWidget::setDataBits(QSerialPort::DataBits dataBits)
{
    ui->dataBits->blockSignals(true);
    int index = ui->dataBits->findData(dataBits);
    if (index)
    {
        ui->dataBits->setCurrentIndex(index);
    }
    ui->dataBits->blockSignals(false);
}

void SerialPortPickerWidget::setParity(QSerialPort::Parity parity)
{
    ui->parity->blockSignals(true);
    int index = ui->parity->findData(parity);
    if (index)
    {
        ui->parity->setCurrentIndex(index);
    }
    ui->parity->blockSignals(false);
}

const QSerialPortInfo& SerialPortPickerWidget::setDevice(const QSerialPortInfo& info)
{
    m_deviceName = info.portName();

    ui->productId->setText(QString::number(info.productIdentifier()));
    ui->vendorId->setText(QString::number(info.vendorIdentifier()));
    ui->manufacturer->setText(info.manufacturer());
    ui->serialNumber->setText(info.serialNumber());
    ui->description->setText(info.description());

    return info;
}

void SerialPortPickerWidget::scanDevices()
{
    m_deviceList.clear();

    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();

    qDebug() << availablePorts.size();

    if (!availablePorts.size())
    {
        QMessageBox::warning(this, "No Serial Ports Found", "No serial ports could be found on this workstation.");
        return;
    }

    for(int i = 0; i < availablePorts.size(); ++i) {
        const QSerialPortInfo& info = availablePorts[i];
        QString label = info.portName();

        if(!m_deviceList.contains(label))
        {
            m_deviceList.insert(label,info);
            ui->serialPorts->addItem(label);
        }
    }
}
