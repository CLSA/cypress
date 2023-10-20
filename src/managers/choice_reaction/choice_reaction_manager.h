#ifndef CHOICE_REACTION_MANAGER_H
#define CHOICE_REACTION_MANAGER_H

#include "managers/manager_base.h"
#include "data/choice_reaction/tests/choice_reaction_test.h"

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
    explicit ChoiceReactionManager(const CypressSession& session);

    static bool isAvailable();
    static bool isInstalled();

    static QString CCB_PREFIX;
    static QString CCB_CLINIC;

    // is the passed string an executable file
    // with the correct path elements ?
    //
    bool isDefined(const QString&) const;

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
    QString m_runnableName; // full pathspec to CCB.exe
    QString m_runnablePath; // path to CCB.exe

    QString m_outputPath;     // path to output .csv files
    QString m_outputFile;     // full pathspec to working output .csv file
    QProcess m_process;

    ChoiceReactionTest* m_test;

    // Reset the session
    bool clearData() override;

    // set input parameters for the test
    void setInputData(const QVariantMap& inputData) override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;

    void configureProcess();
};

#endif // CHOICE_REACTION_MANAGER_H
