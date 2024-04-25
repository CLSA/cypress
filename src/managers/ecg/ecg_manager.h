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

static const QString INIT_PATH = "initecg";
static const QString DATABASE_PATH = "DATABASE";

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
    bool start() override;

    // retrieve a measurement from the device
    //
    void measure() override;

    void finish() override;

    void readOutput() override;

private:
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



    QProcess m_process;

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;

    bool backupData();
    bool restoreBackup();

    void configureProcess();
};

#endif // ECG_MANAGER_H
