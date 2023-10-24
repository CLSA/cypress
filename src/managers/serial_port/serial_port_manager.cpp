#include <QDebug>
#include <QJsonObject>
#include <QSettings>

#include "serial_port_manager.h"

SerialPortManager::SerialPortManager(QSharedPointer<CypressSession> session)
    : ManagerBase(session)
{
    connect(&m_port, &QSerialPort::readyRead,
            this,    &SerialPortManager::readDevice);

    connect(&m_port, &QSerialPort::errorOccurred,
            this,    &SerialPortManager::handleSerialPortError);

    connect(&m_port, &QSerialPort::dataTerminalReadyChanged,
            this,    &SerialPortManager::handleDataTerminalReadyChanged);

    connect(&m_port, &QSerialPort::requestToSendChanged,
            this,    &SerialPortManager::handleRequestToSendChanged);
}

void SerialPortManager::start()
{
    scanDevices();
    //qDebug() << "signal dataChanged";
    //emit dataChanged();
}

void SerialPortManager::handleSerialPortError(QSerialPort::SerialPortError error)
{
    if(error == QSerialPort::NoError)
    {
        return;
    }

    qCritical() << "ERROR: serial port " << m_port.errorString();
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
    qDebug() << "SerialPortManager::isDefined - " << label;

    if(m_deviceList.contains(label))
    {
        QSerialPortInfo info = m_deviceList.value(label);
        return !info.isNull();
    }

    return false;
}

QJsonObject SerialPortManager::getDeviceData(const QSerialPortInfo& info)
{
    qDebug() << "SerialPortManager::getDeviceData";

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
    qDebug() << "SerialPortManager::getDeviceData";
    m_deviceData = deviceData;
}

bool SerialPortManager::scanDevices()
{
    qDebug() << "SerialPortManager::scanningDevices";

    m_deviceList.clear();
    emit scanningDevices();

    foreach(const auto info, QSerialPortInfo::availablePorts())
    {
        const QJsonObject deviceData = getDeviceData(info);
        if (!deviceData.contains("port_number"))
            continue;

        const QString portNumber = deviceData["port_number"].toString();
        if (!m_deviceList.contains(portNumber))
        {
            m_deviceList.insert(portNumber, info);
            //emit deviceDiscovered(portNumber);
        }
    }

    qInfo() << "found" << QString::number(m_deviceList.count()) << " serial ports";

    emit devicesDiscovered(m_deviceList);

    // if we have a port from the ini file, check if it is still available on the system
    //
    //bool found = false;
    //QSerialPortInfo info;
    //QString label;
    //if(!m_deviceName.isEmpty())
    //{
    //    QMap<QString,QSerialPortInfo>::const_iterator it = m_deviceList.constBegin();
    //    while(it != m_deviceList.constEnd() && !found)
    //    {
    //      label = it.key();
    //      found = label == m_deviceName;
    //      if(found) info = it.value();
    //      ++it;
    //    }
    //}
    //if(found)
    //{
    //    qInfo() << "found port device" << m_deviceName;

    //  qDebug() << "signal deviceSelected";
    //  emit deviceSelected(label);
    //  setDevice(info);
    //}
    //else
    //{
    //  // select a serial port from the list of scanned ports
    //  //
    //  qDebug() << "signal canSelectDevice";
    //  emit canSelectDevice();
    //}

    return true;
}

void SerialPortManager::selectDevice(const QString &label)
{
    qDebug() << "SerialPortManager::selectDevice - " << label;
    if(m_deviceList.contains(label))
    {
      QSerialPortInfo info = m_deviceList.value(label);
      setProperty("deviceName",info.portName());
      setDevice(info);
      qDebug() << "SerialPortManager:: port selected " <<  label;
    }
}

void SerialPortManager::setDevice(const QSerialPortInfo &info)
{
    qDebug() << "SerialPortManager::setDevice";
    m_port.setPort(info);
    if(m_port.open(QSerialPort::ReadWrite))
    {
      emit canConnectDevice();
      m_port.close();
    }
}

void SerialPortManager::connectDevice()
{
    qDebug() << "SerialPortManager::connectDevice";
    if(m_port.isOpen())
        m_port.close();

    if(m_port.open(QSerialPort::ReadWrite))
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
    qDebug() << "SerialPortManager::disconnectDevice";
    if(m_port.isOpen())
        m_port.close();

    emit canConnectDevice();
}

// set input parameters for the test
void SerialPortManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
}


bool SerialPortManager::setUp()
{
    return true;
}

bool SerialPortManager::clearData()
{
    return true;
}

bool SerialPortManager::cleanUp()
{
    return true;
}
