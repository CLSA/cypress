#ifndef TONOMETER_MANAGER_H
#define TONOMETER_MANAGER_H

#include "data/tonometer/tests/tonometer_test.h"
#include "managers/manager_base.h"
#include "server/sessions/tonometer_session.h"

#include <QProcess>

/*!
 * \class TonometerManager
 * \brief The TonometerManager class
 *
 * Concrete child class implementation of a device manager.
 * This class facilitates launch of the Reichert ORA software
 * (ora.exe) and retrieval of test data from its MS Access database
 * (ora.mdb).  QProcess is used to facilitate exe operations
 * along with helper class AccessQueryHelper to read the MS Access file.
 *
 * \sa ManagerBase, CDTTManager, AccessQueryHelper
 *
 */

class TonometerManager : public ManagerBase
{
    enum FileType {
        ORAApplication,
        ORADatabase
    };

    Q_OBJECT

public:
    explicit TonometerManager(QSharedPointer<TonometerSession> session);
    ~TonometerManager();

    // is the passed string an executable file
    // with the correct path elements ?
    //
    bool isDefined(const QString&, const TonometerManager::FileType& type = ORAApplication) const;

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

    QString m_runnableName;// full pathspec to ora.exe
    QString m_runnablePath;// path to ora.exe
    QString m_databaseName;// full pathspec to ora.mdb
    QString m_temporaryFile; // store a copy of ora.mdb

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;

    void configureProcess();
};

#endif // TONOMETER_MANAGER_H
