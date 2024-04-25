#include <QDebug>
#include <QJsonObject>
#include <QSettings>

#include "serial_port_manager.h"

SerialPortManager::SerialPortManager(QSharedPointer<CypressSession> session)
    : ManagerBase(session)
{

}

bool SerialPortManager::start()
{
    qInfo() << "SerialPortManager::start";
    scanDevices();
    return true;
}

void SerialPortManager::handleSerialPortError(QSerialPort::SerialPortError error)
{
    qCritical() << "ERROR: serial port " << m_port.errorString();

    if(error == QSerialPort::NoError)
        return;
}

void SerialPortManager::handleDataTerminalReadyChanged(bool set)
{
    qDebug() << "SerialPortManager::handleDataTerminalReadyChanged - " << (set ? "high" : "low");
}

void SerialPortManager::handleRequestToSendChanged(bool set)
{
    qDebug() << "SerialPortManager::handleRequestToSendChanged - " << (set ? "high" : "low");
}

bool SerialPortManager::isDefined(const QString &label) const
{
    qInfo() << "SerialPortManager::isDefined - " << label;

    if(m_deviceList.contains(label))
    {
        QSerialPortInfo info = m_deviceList.value(label);
        return !info.isNull();
    }

    return false;
}

QJsonObject SerialPortManager::getDeviceData(const QSerialPortInfo& info)
{
    qInfo() << "SerialPortManager::getDeviceData";

    QJsonObject deviceData {{}};

    if(info.hasProductIdentifier())
        deviceData["product_id"] = QString::number(info.productIdentifier());

    if(info.hasVendorIdentifier())
        deviceData["vendor_id"] = QString::number(info.vendorIdentifier());

    if(!info.manufacturer().isEmpty())
        deviceData["manufacturer"] = info.manufacturer();

    if(!info.portName().isEmpty())
        deviceData["port_name"] = info.portName();

    if(!info.serialNumber().isEmpty())
        deviceData["serial_number"] = info.portName();

    if(!info.systemLocation().isEmpty())
        deviceData["system_location"] = info.systemLocation();

    if(!info.description().isEmpty())
        deviceData["description"] = info.description();

    return deviceData;
}

void SerialPortManager::setDeviceData(const QJsonObject& deviceData)
{
    qInfo() << "SerialPortManager::getDeviceData";

    m_deviceData = deviceData;
}

bool SerialPortManager::scanDevices()
{
    qInfo() << "SerialPortManager::scanningDevices";

    m_deviceList.clear();
    emit scanningDevices();

    foreach(const auto info, QSerialPortInfo::availablePorts())
        m_deviceList.insert(info.portName(), info);

    emit devicesDiscovered(m_deviceList);

    // if we have a port from the ini file, check if it is still available on the system
    //
    bool found = false;

    QSerialPortInfo info;
    QString label;

    if(!m_portName.isEmpty())
    {
        QMap<QString,QSerialPortInfo>::const_iterator it = m_deviceList.constBegin();
        while(it != m_deviceList.constEnd() && !found)
        {
          label = it.key();
            found = label == m_portName;
          if(found) info = it.value();
          ++it;
        }
    }

    if(found)
    {
        qInfo() << "Found port device: " << m_portName;

        selectDevice(info);
        emit deviceSelected(info);
    }
    else
    {
        // select a serial port from the list of scanned ports
        //
        emit canSelectDevice();
    }

    return true;
}

void SerialPortManager::selectDevice(const QSerialPortInfo &port)
{
    qInfo() << "SerialPortManager::selectDevice: " << port.portName();

    setProperty("deviceName", port.portName());
    setDevice(port);
}

void SerialPortManager::setDevice(const QSerialPortInfo &info)
{
    qInfo() << "SerialPortManager::setDevice" << info.portName();

    m_port.setPort(info);
    if (m_port.open(QSerialPort::ReadWrite))
    {
        emit canConnectDevice();
        m_port.close();
    }
}

void SerialPortManager::connectDevice()
{
    qInfo() << "SerialPortManager::connectDevice";

    if(m_port.isOpen())
    {
        qDebug() << "closing port";

        m_port.close();

        disconnect(&m_port, &QSerialPort::readyRead,
            this,    &SerialPortManager::readDevice);

        disconnect(&m_port, &QSerialPort::errorOccurred,
            this,    &SerialPortManager::handleSerialPortError);

        disconnect(&m_port, &QSerialPort::dataTerminalReadyChanged,
            this,    &SerialPortManager::handleDataTerminalReadyChanged);

        disconnect(&m_port, &QSerialPort::requestToSendChanged,
            this,    &SerialPortManager::handleRequestToSendChanged);

    }

    connect(&m_port, &QSerialPort::readyRead,
            this,    &SerialPortManager::readDevice);

    connect(&m_port, &QSerialPort::errorOccurred,
            this,    &SerialPortManager::handleSerialPortError);

    connect(&m_port, &QSerialPort::dataTerminalReadyChanged,
            this,    &SerialPortManager::handleDataTerminalReadyChanged);

    connect(&m_port, &QSerialPort::requestToSendChanged,
            this,    &SerialPortManager::handleRequestToSendChanged);

    if (m_port.open(QSerialPort::ReadWrite))
    {
        m_port.setDataBits(QSerialPort::Data8);
        m_port.setParity(QSerialPort::NoParity);
        m_port.setStopBits(QSerialPort::OneStop);
        m_port.setBaudRate(QSerialPort::Baud9600);

        // signal the GUI that the measure button can be clicked
        //
        emit canMeasure();
    }
}

void SerialPortManager::disconnectDevice()
{
    qInfo() << "SerialPortManager::disconnectDevice";

    if (m_port.isOpen())
        m_port.close();

    disconnect(&m_port, &QSerialPort::readyRead,
            this,    &SerialPortManager::readDevice);

    disconnect(&m_port, &QSerialPort::errorOccurred,
            this,    &SerialPortManager::handleSerialPortError);

    disconnect(&m_port, &QSerialPort::dataTerminalReadyChanged,
            this,    &SerialPortManager::handleDataTerminalReadyChanged);

    disconnect(&m_port, &QSerialPort::requestToSendChanged,
            this,    &SerialPortManager::handleRequestToSendChanged);

    emit cannotMeasure();
    emit deviceDisconnected();
    emit canConnectDevice();
}

bool SerialPortManager::setUp()
{
    qInfo() << "SerialPortManager::setUp";
    return true;
}

bool SerialPortManager::clearData()
{
    qInfo() << "SerialPortManager::clearData";
    return true;
}

bool SerialPortManager::cleanUp()
{
    qInfo() << "SerialPortManager::cleanUp";
    return true;
}
