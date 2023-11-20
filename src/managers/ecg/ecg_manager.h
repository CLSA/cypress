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
    bool isDefined(const QString&, const FileType& type = ECGApplication) const;

    // full pathspec to Cardiosoft Cardio.exe
    //
    QString m_runnableName;

    // path to backup db files
    // C:/CARDIO
    //
    QString m_workingPath;

    // C:/CARDIO/Export
    //
    QString m_exportPath;

    // full pathspec to exported xml file
    //
    QString m_outputFile;

    // path for Cardiosoft database backup to and restore from
    //
    const QString INIT_PATH = "initecg";
    const QString DATABASE_PATH = "DATABASE";


    QProcess m_process;

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;

    bool deleteDeviceData();

    void configureProcess();
};

#endif // ECG_MANAGER_H
