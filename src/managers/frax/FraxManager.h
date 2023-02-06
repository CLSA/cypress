#ifndef FRAXMANAGER_H
#define FRAXMANAGER_H

#include "managers/ManagerBase.h"
#include "data/frax/tests/FraxTest.h"
#include <QProcess>

/*!
 * \class FraxManager
 * \brief The FraxManager class
 *
 * Concrete child class implementation of a device manager.
 * This class facilitates launch of the Frax (fracture risk assessment)
 * module (blackbox.exe) and reading the test output
 * .txt files it produces.  QProcess is used to facilitate operations.
 *
 * \sa ManagerBase, CDTTManager, ChoiceReactionManager
 *
 */

class FraxManager : public ManagerBase
{
    Q_OBJECT

public:
    explicit FraxManager();

    // is the passed string an executable file
    // with the correct path elements ?
    //
    bool isDefined(const QString&) const;
    bool isAvailable() override;

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

private:
    QString m_runnableName;// full pathspec to blackbox.exe
    QString m_runnablePath;// path to blackbox.exe

    QString m_outputFile;    // full pathspec to working output.txt
    QString m_inputFile;     // full pathspec to working input.txt
    QString m_temporaryFile; // store a copy of the default input.txt
    QProcess m_process;

    FraxTest m_test;

    void clearData() override;

    void configureProcess();
};

#endif // FRAXMANAGER_H
