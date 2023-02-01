#include "SerialPortManager.h"

#include <QDebug>
#include <QJsonObject>
#include <QSettings>

SerialPortManager::SerialPortManager(QWidget *parent) : ManagerBase(parent)
{
}

void SerialPortManager::start()
{
    qDebug() << "slot start";
    connect(&m_port, &QSerialPort::readyRead,
      this, &SerialPortManager::readDevice);

    connect(&m_port, &QSerialPort::errorOccurred,
      this,[this](QSerialPort::SerialPortError error)
      {
        if(error == QSerialPort::NoError)
          return;

        qCritical() << "ERROR: serial port " << m_port.errorString();
      });

    connect(&m_port, &QSerialPort::dataTerminalReadyChanged,
      this,[](bool set)
      {
        qDebug() << "data terminal ready DTR changed to " << (set?"high":"low");
      });

    connect(&m_port, &QSerialPort::requestToSendChanged,
      this,[](bool set)
      {
        qDebug() << "request to send RTS changed to " << (set?"high":"low");
      });

  scanDevices();
  qDebug() << "signal dataChanged";
  emit dataChanged();
}

bool SerialPortManager::isDefined(const QString &label) const
{
    bool defined = false;
    if(m_deviceList.contains(label))
    {
        QSerialPortInfo info = m_deviceList.value(label);
        defined = !info.isNull();
    }
    return defined;
}

bool SerialPortManager::scanDevices()
{
    m_deviceList.clear();
    qDebug() << "signal scanningDevices";
    emit scanningDevices();

    foreach(const auto info, QSerialPortInfo::availablePorts())
    {
      // get the device data
      if(info.hasProductIdentifier())
        qInfo() << "product id:" << QString::number(info.productIdentifier());
      if(info.hasVendorIdentifier())
        qInfo() << "vendor id:" << QString::number(info.vendorIdentifier());
      if(!info.manufacturer().isEmpty())
        qInfo() << "manufacturer:" << info.manufacturer();
      if(!info.portName().isEmpty())
        qInfo() << "port name:" << info.portName();
      if(!info.serialNumber().isEmpty())
        qInfo() << "serial number:" << info.serialNumber();
      if(!info.systemLocation().isEmpty())
        qInfo() << "system location:" << info.systemLocation();
      if(!info.description().isEmpty())
        qInfo() << "description:" << info.description();
        QString label = info.portName();
        if(!m_deviceList.contains(label))
        {
            m_deviceList.insert(label,info);
            qDebug() << "signal deviceDiscovered";
            emit deviceDiscovered(label);
        }
    }
        qInfo() << "found" << QString::number(m_deviceList.count()) << " serial ports";

    // if we have a port from the ini file, check if it is still available on the system
    //
    bool found = false;
    QSerialPortInfo info;
    QString label;
    if(!m_deviceName.isEmpty())
    {
        QMap<QString,QSerialPortInfo>::const_iterator it = m_deviceList.constBegin();
        while(it != m_deviceList.constEnd() && !found)
        {
          label = it.key();
          found = label == m_deviceName;
          if(found) info = it.value();
          ++it;
        }
    }
    if(found)
    {
        qInfo() << "found port device" << m_deviceName;

      qDebug() << "signal deviceSelected";
      emit deviceSelected(label);
      setDevice(info);
    }
    else
    {
      // select a serial port from the list of scanned ports
      //
      qDebug() << "signal canSelectDevice";
      emit canSelectDevice();
    }

    return found;
}

void SerialPortManager::selectDevice(const QString &label)
{
    qDebug() << "slot selectDevice";
    if(m_deviceList.contains(label))
    {
      QSerialPortInfo info = m_deviceList.value(label);
      setProperty("deviceName",info.portName());
      setDevice(info);
      qInfo() << "device selected from label " <<  label;
    }
}

void SerialPortManager::setDevice(const QSerialPortInfo &info)
{
    qDebug() << "slot setDevice";
    m_deviceData.reset();

    if(m_deviceName.isEmpty() || info.isNull())
    {
        qCritical() << "ERROR: no port available";
        return;
    }

    m_port.setPort(info);
    if(m_port.open(QSerialPort::ReadWrite))
    {
      // get the device data
      if(info.hasProductIdentifier())
        m_deviceData.setAttribute("port_product_id", info.productIdentifier());
      if(info.hasVendorIdentifier())
        m_deviceData.setAttribute("port_vendor_id", info.vendorIdentifier());
      if(!info.manufacturer().isEmpty())
        m_deviceData.setAttribute("port_manufacturer", info.manufacturer());
      if(!info.portName().isEmpty())
        m_deviceData.setAttribute("port_name", info.portName());
      if(!info.serialNumber().isEmpty())
        m_deviceData.setAttribute("port_serial_number", info.serialNumber());
      if(!info.systemLocation().isEmpty())
        m_deviceData.setAttribute("port_system_location", info.systemLocation());
      if(!info.description().isEmpty())
        m_deviceData.setAttribute("port_description", info.description());

      // signal the GUI that the port is connectable so that
      // the connect button can be clicked
      //
      qDebug() << "signal canConnectDevice";
      emit canConnectDevice();
      m_port.close();
    }
}

void SerialPortManager::connectDevice()
{
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
      qDebug() << "signal canMeasure";
      emit canMeasure();
    }
}

void SerialPortManager::disconnectDevice()
{
    qDebug() << "signal canConnectDevice";
    emit canConnectDevice();

    if(m_port.isOpen())
        m_port.close();
}
