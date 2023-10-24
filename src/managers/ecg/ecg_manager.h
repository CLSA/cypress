#ifndef ECG_MANAGER_H
#define ECG_MANAGER_H

#include <QProcess>

#include "data/ecg/tests/ecg_test.h"
#include "managers/manager_base.h"
#include "server/sessions/ecg_session.h"

/*!
 * \class ECGManager
 * \brief The ECGManager class
 *
 * Concrete child class implementation of a device manager.
 * This class facilitates launch of Cardiosoft
 * and reading the test output .xml files it produces.
 * QProcess is used to facilitate operations.
 *
 * \sa ManagerBase, CDTTManager, TonometerManager
 *
 */

class ECGManager : public ManagerBase
{
    enum FileType {
        ECGApplication,
        ECGWorkingDir
    };

    Q_OBJECT

public:
    explicit ECGManager(QSharedPointer<ECGSession> session);

    static bool isAvailable();
    static bool isInstalled();

public slots:

    // what the manager does in response to the main application
    // window invoking its run method
    //
    void start() override;

    // retrieve a measurement from the device
    //
    void measure() override;

    // implementation of final clean up of device after disconnecting and all
    // data has been retrieved and processed by any upstream classes
    //
    void finish() override;

    void readOutput();

private:
    QProcess m_process;

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;

    // set input parameters for the test
    void setInputData(const QVariantMap& inputData) override;

    bool deleteDeviceData();

    void configureProcess();
};

#endif // ECG_MANAGER_H
