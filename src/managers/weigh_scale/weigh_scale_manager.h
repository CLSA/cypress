#ifndef WEIGH_SCALE_MANAGER_H
#define WEIGH_SCALE_MANAGER_H

#include "managers/serial_port/serial_port_manager.h"
#include "server/sessions/weigh_scale_session.h"
#include "config/device_config.h"

/*!
 * \class WeighScaleManager
 * \brief The WeighScaleManager class
 *
 * Concrete child class implementation of a RS232 serial
 * device manager.  This class facilitates connection to a
 * Rice Lake digital weigh scale for read/write instructions
 * and test data retrieval.
 *
 * \sa ManagerBase, SerialPortManager, BodyCompositionManager
 *
 */

class WeighScaleManager : public SerialPortManager
{
    Q_OBJECT

public:
    explicit WeighScaleManager(QSharedPointer<WeighScaleSession> &session);
    static DeviceConfig config;

public slots:
    // connect to the serial port
    // opens the serial port with required parametere (baud rate etc.)
    // connects the port readyRead signal to the readDevice slot
    // emits canMeasure signal if the port is open
    //
    void connectDevice() override;

    void selectDevice(const QSerialPortInfo &port) override;

    // zero the weigh scale
    //
    void zeroDevice();

    bool start() override;

    // retrieve a measurement from the device
    //
    void measure() override;

    void addManualEntry(const double weight);
    void removeMeasurement(const int index);

private slots:

    // retrieve data from the scale over RS232
    // emits canFinish signal if the test data is valid
    // Read is based on the last written code
    //
    void readDevice() override;

    void writeDevice() override;

private:
    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;
};

#endif // WEIGH_SCALE_MANAGER_H
