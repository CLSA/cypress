#include "BluetoothLEManager.h"

#include <QBitArray>
#include <QDateTime>
#include <QtBluetooth/QBluetoothHostInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>
#include <QStandardItemModel>

/**
 * Qt 5.14 adds a native Win32 port supporting Classic Bluetooth on Windows 7 or newer, and Bluetooth LE on Windows 8 or newer.
 * It must be enabled at build time by configuration option -native-win32-bluetooth. The UWP backend is used by default if this
 * option is not set and the Win32 target platform supports the required UWP APIs (minimal requirement is Windows 10 version 1507,
 * with slightly improved service discovery since Windows 10 version 1607).
 *
*/

BluetoothLEManager::BluetoothLEManager()
{
    m_test.setExpectedMeasurementCount(2);
}

void BluetoothLEManager::start()
{
    scanDevices();
    emit dataChanged();
}

bool BluetoothLEManager::isAvailable()
{
    return false;
}

void BluetoothLEManager::setLocalDevice(const QString &address)
{
    Q_UNUSED(&address)
#ifdef Q_OS_LINUX
    if(!address.isEmpty())
    {
      m_localDevice.reset(new QBluetoothLocalDevice(QBluetoothAddress(address)));
      if(m_verbose)
          qDebug() << "using local adapter from settings " << address;
    }
    if(m_localDevice.isNull())
    {
      QList<QBluetoothHostInfo> localAdapters = QBluetoothLocalDevice::allDevices();
      if(!localAdapters.empty())
      {
        m_localDevice.reset(new QBluetoothLocalDevice(localAdapters.at(0).address()));
        if(m_verbose)
            qDebug() << "using first local adapter from list of known devices " << m_localDevice->address().toString();
      }
    }

    if(m_localDevice->isValid())
    {
      if(QBluetoothLocalDevice::HostPoweredOff == m_localDevice->hostMode())
      {
        if(m_verbose)
            qDebug() << "local adapter is powered off: powering on";
        m_localDevice->powerOn();
      }
      if(QBluetoothLocalDevice::HostDiscoverable != m_localDevice->hostMode())
      {
        if(m_verbose)
            qDebug() << "setting local adapter host mode to host discoverable";
        m_localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);
      }
    }
    else
    {
      if(m_verbose)
        qDebug() << "error: could not find a local adapter";
    }
 #endif
}

bool BluetoothLEManager::lowEnergyEnabled() const
{
  return (QBluetoothDeviceDiscoveryAgent::LowEnergyMethod &
          QBluetoothDeviceDiscoveryAgent::supportedDiscoveryMethods());
}

bool BluetoothLEManager::localDeviceEnabled() const
{
    bool enabled = true;
#ifdef Q_OS_LINUX
    enabled = (!m_localDevice.isNull() && m_localDevice->isValid());
#endif
    return enabled;
}

void BluetoothLEManager::scanDevices()
{
    m_deviceList.clear();
    emit scanningDevices();

    // NOTE: Due to API limitations it is only possible to find devices that have been paired
    // using Windows' settings on Win OS.
    // Create the agent to perform device discovery and populate the address list box
    // with candidate items.
    // If the address line edit field has not been filled with a stored peripheral address,
    // prompt the user to double click to select a device.
    //
    if(m_agent.isNull())
    {
      m_agent.reset(new QBluetoothDeviceDiscoveryAgent(this));

      connect(m_agent.data(), &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
           this, &BluetoothLEManager::deviceDiscoveredInternal);

      connect(m_agent.data(), QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            this,[](QBluetoothDeviceDiscoveryAgent::Error error)
        {
          QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"),Qt::SkipEmptyParts);
        }
      );

      connect(m_agent.data(), &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BluetoothLEManager::discoveryCompleteInternal);

      connect(m_agent.data(), &QBluetoothDeviceDiscoveryAgent::canceled,
              this, &BluetoothLEManager::discoveryCompleteInternal);
    }

    m_agent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

bool BluetoothLEManager::isPairedTo(const QString &label) const
{
    Q_UNUSED(label)
    return true;
}

// add a device to the ui list
//
void BluetoothLEManager::deviceDiscoveredInternal(const QBluetoothDeviceInfo &info)
{
    Q_UNUSED(info)
}

// Enable discovered devices selection
//
void BluetoothLEManager::discoveryCompleteInternal()
{

}

// select the bluetooth peripheral device
//
void BluetoothLEManager::selectDevice(const QString &label)
{
    Q_UNUSED(label)
}

void BluetoothLEManager::serviceDiscovered(const QBluetoothUuid &uuid)
{
    Q_UNUSED(uuid)
}

//bool BluetoothLEManager::clearData()
//{
//    return false;
//}

void BluetoothLEManager::finish()
{

}

void BluetoothLEManager::setDevice(const QBluetoothDeviceInfo &info)
{
    Q_UNUSED(info)
}

void BluetoothLEManager::connectDevice()
{

}

void BluetoothLEManager::serviceDiscoveryComplete()
{

}

void BluetoothLEManager::measure()
{

}

void BluetoothLEManager::infoServiceStateChanged(QLowEnergyService::ServiceState state)
{
    Q_UNUSED(state)
}

void BluetoothLEManager::thermoServiceStateChanged(QLowEnergyService::ServiceState state)
{
    Q_UNUSED(state)
}

void BluetoothLEManager::disconnectDevice()
{

}

void BluetoothLEManager::updateInfoData(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    Q_UNUSED(c)
    Q_UNUSED(value)
}

void BluetoothLEManager::updateTemperatureData(const QLowEnergyCharacteristic &c, const QByteArray &arr)
{
    Q_UNUSED(c)
    Q_UNUSED(arr)
}

// Set up device
bool BluetoothLEManager::setUp()
{
    return true;
}

// Clean up the device for next time
bool BluetoothLEManager::cleanUp()
{
    return true;
}

bool BluetoothLEManager::clearData()
{
    return true;
}

void BluetoothLEManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
}
