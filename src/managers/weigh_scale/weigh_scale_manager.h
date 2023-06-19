#ifndef WEIGH_SCALE_MANAGER_H
#define WEIGH_SCALE_MANAGER_H

#include "managers/serial_port/serial_port_manager.h"
#include "data/weigh_scale/tests/weigh_scale_test.h"

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
    explicit WeighScaleManager(QJsonObject inputData);

    bool isAvailable();

public slots:
    // connect to the serial port
    // opens the serial port with required parametere (baud rate etc.)
    // connects the port readyRead signal to the readDevice slot
    // emits canMeasure signal if the port is open
    //
    void connectDevice() override;

    // zero the weigh scale
    //
    void zeroDevice();

    // retrieve a measurement from the device
    //
    void measure() override;

    // implementation of final clean up of device after disconnecting and all
    // data has been retrieved and processed by any upstream classes
    //
    void finish() override;

private slots:

    // retrieve data from the scale over RS232
    // emits canFinish signal if the test data is valid
    // Read is based on the last written code
    //
    void readDevice() override;

    void writeDevice() override;

private:
    WeighScaleTest m_test;

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;

    // set input parameters for the test
    void setInputData(const QVariantMap& inputData) override;
};

#endif // WEIGH_SCALE_MANAGER_H
