#ifndef CHOICEREACTIONMANAGER_H
#define CHOICEREACTIONMANAGER_H

#include "ManagerBase.h"
#include "../data/ChoiceReactionTest.h"
#include <QProcess>

/*!
 * \class ChoiceReactionManager
 * \brief The ChoiceReactionManager class
 *
 * Concrete child class implementation of a device manager.
 * This class facilitates launch of the Cardiff Cognitive Battery
 * choice reaction test program (CCB.exe) and reading the test output
 * .csv files it produces.   QProcess is used to facilitate operations.
 *
 * \sa ManagerBase, CDTTManager, FraxManager
 *
 */

class ChoiceReactionManager: public ManagerBase
{
    Q_OBJECT

public:
    explicit ChoiceReactionManager(QObject* parent = Q_NULLPTR);

    static QString CCB_PREFIX;
    static QString CCB_CLINIC;

    void loadSettings(const QSettings&) override;
    void saveSettings(QSettings*) const override;

    QJsonObject toJsonObject() const override;

    void initializeModel() override;

    void updateModel() override;

    // is the passed string an executable file
    // with the correct path elements ?
    //
    bool isDefined(const QString&) const;

    // Set the input data.
    // The input data is read from the input
    // json file to the main application.  This method should be
    // used to filter the minimum inputs needed to run
    // a test.  Filtering keys are stored in member
    // m_inputKeyList.
    //
    void setInputData(const QVariantMap&) override;

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
    QString m_runnableName; // full pathspec to CCB.exe
    QString m_runnablePath; // path to CCB.exe

    QString m_outputPath;     // path to output .csv files
    QString m_outputFile;     // full pathspec to working output .csv file
    QProcess m_process;

    ChoiceReactionTest m_test;

    void clearData() override;

    void configureProcess();
};

#endif // CHOICEREACTIONMANAGER_H
