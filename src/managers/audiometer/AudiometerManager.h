#ifndef AUDIOMETERMANAGER_H
#define AUDIOMETERMANAGER_H

#include "managers/serial_port/SerialPortManager.h"
#include "data/hearing/tests/HearingTest.h"

/*!
 * \class AudiometerManager
 * @brief The AudiometerManager class
 *
 * Concrete child class implementation of a RS232 serial
 * device manager.  This class facilitates connection to a
 * Tremetrics RA300 or RA300 Plus audiometer for read instructions
 * and test data retrieval.
 *
 * Caveats:
 * RA300 or RA300 Plus must be configured as follows:
 * - pulsed stimulus mode [special 2]
 * - baud rate set to 9600 [special 7]
 * - RA500 interface [special 5]
 *
 * \sa ManagerBase, SerialPortManager, BodyCompositionManager
 *
 */

class AudiometerManager : public SerialPortManager
{
    Q_OBJECT

public:
    explicit AudiometerManager();

    static QByteArray initEndCode();
    static QByteArray END_CODE;
    static bool isInstalled();

public slots:
    // retrieve a measurement from the device
    //
    void measure() override;

    // implementation of final clean up of device after disconnecting and all
    // data has been retrieved and processed by any upstream classes
    //
    void finish() override;

private slots:
    // retrieve data from the audiometer over RS232
    // emits canFinish signal if the test data is valid
    //
    void readDevice() override;
    void writeDevice() override;

private:
    HearingTest m_test;

    bool hasEndCode(const QByteArray&);

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;

    // set input parameters for the test
    void setInputData(const QVariantMap& inputData) override;
};

#endif // AUDIOMETERMANAGER_H
