#ifndef ECGMANAGER_H
#define ECGMANAGER_H

#include "managers/ManagerBase.h"
#include "data/ecg/tests/ECGTest.h"
#include <QProcess>

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
    explicit ECGManager();

    // is the passed string an executable file
    // with the correct path elements ?
    //
    bool isDefined(const QString&, const FileType& type = ECGApplication) const;

    bool isAvailable();

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

    // set the executable full path and name
    // calls isDefined to validate the passed arg
    //
    void selectRunnable(const QString&);

    void selectWorking(const QString&);

    void select();

    void readOutput();

signals:

    // a valid runnable was selected
    // manager attempts to configure the process and may emit canMeasure on success
    //
    void runnableSelected();

    // no runnable available or the selected runnable is invalid
    // signal can be connected to a ui slot to launch a File select dialog
    //
    void canSelectRunnable();

    void workingSelected();

    void canSelectWorking();

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

    QProcess m_process;

    ECGTest m_test;

    // path for Cardiosoft database backup to and restore from
    //
    const QString INIT_PATH = "initecg";
    const QString DATABASE_PATH = "DATABASE";

    bool deleteDeviceData();
    bool clearData();
    void configureProcess();
};

#endif // ECGMANAGER_H
