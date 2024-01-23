#ifndef SERIAL_PORT_MANAGER_H
#define SERIAL_PORT_MANAGER_H

#include <QSerialPort>
#include <QSerialPortInfo>

#include "managers/manager_base.h"
#include "data/measurement.h"

/*!
 * \class SerialPortManager
 * \brief The SerialPortManager class
 *
 * Abstract parent class for all devices that communicate over
 * serial RS232.  This class manages device selection, connectivity
 * and communication with serial port devices.
 *
 * \sa ManagerBase, AudiometerManager, BodyCompositionManager
 *
*/

class SerialPortManager : public ManagerBase
{
    Q_OBJECT

    Q_PROPERTY(QString deviceName MEMBER m_deviceName NOTIFY deviceNameChanged)

public:
    explicit SerialPortManager(QSharedPointer<CypressSession> session);

    bool isDefined(const QString&) const;

public slots:
    // what the manager does in response to the main application
    // window invoking its run method
    //
    bool start() override;

    // connect to the serial port
    // opens the serial port with required parametere (baud rate etc.)
    // connects the port readyRead signal to the readDevice slot
    // emits canMeasure signal if the port is open
    //
    virtual void connectDevice();

    // disconnect from the serial port
    //
    void disconnectDevice();

    // select a device (serial port) by name
    // checks of the named port is in the list of scanned devices
    // and calls setDevice
    //
    void selectDevice(const QSerialPortInfo& port);

protected slots:

    // send write request over RS232 to retrieve data from the audiometer
    //
    virtual void writeDevice() = 0;

    // retrieve data from the audiometer over RS232
    // emits canFinish signal if the test data is valid
    //
    virtual void readDevice() = 0;

    // set the serial port
    //
    void setDevice(const QSerialPortInfo&);

    virtual void handleSerialPortError(QSerialPort::SerialPortError error);
    virtual void handleDataTerminalReadyChanged(bool set);
    virtual void handleRequestToSendChanged(bool set);

signals:

    // port scanning started
    // (update GUI status)
    //
    void scanningDevices();

    // a single port was discovered during the scan process
    // (update GUI list of ports)
    //
    void deviceDiscovered(const QString&);

    // The full list of ports that were discovered
    void devicesDiscovered(const QMap<QString, QSerialPortInfo>&);

    // a list of scanned port devices is avaiable for selection
    // (update GUI to prompt for user to select a port)
    //
    void canSelectDevice();

    // a port was selected from the list of discovered ports
    //
    void deviceSelected(const QString&);

    // port ready to connect
    // (update GUI enable connect button, disable disconnect button)
    //
    void canConnectDevice();

    void deviceNameChanged(const QString&);

    void deviceConnected();
    void deviceDisconnected();


protected:
    // scan for available devices (serial ports)
    // emits scanning signal at start
    // populates a list of devices using serial port name as key
    // emits discovered signal with the serial port name when a port is discovered
    // if the ini stored port is found
    //   setDevice
    // else
    //   emits canSelect signal
    //
    bool scanDevices();

    QJsonObject m_deviceData;

    // list of available serial ports
    QMap<QString,QSerialPortInfo> m_deviceList;

    // the currently selected serial port
    QSerialPort m_port;
    QString m_deviceName;

    // serial port output receive buffer
    QByteArray m_buffer;

    // serial port input signal
    QByteArray m_request;

    QJsonObject getDeviceData(const QSerialPortInfo& info);
    void setDeviceData(const QJsonObject& deviceData);

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;
};

#endif // SERIAL_PORT_MANAGER_H
