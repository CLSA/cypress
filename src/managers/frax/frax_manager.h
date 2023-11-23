#ifndef FRAX_MANAGER_H
#define FRAX_MANAGER_H

#include "managers/manager_base.h"
#include "server/sessions/frax_session.h"

#include "data/frax/tests/frax_test.h"
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
    explicit FraxManager(QSharedPointer<FraxSession> session);

    static bool isInstalled();

public slots:
    void start() override;

    void measure() override;

    void finish() override;

private:
    void readOutput();

    QString m_country_code;
    QString m_type_code;

    QString m_executablePath;		// full path to blackbox.exe
    QString m_workingDirectoryPath; // path to blackbox.exe directory

    QString m_outputFile;    		// full path to output.txt
    QString m_inputFile;     		// full path to input.txt
    QString m_temporaryFile; 		// store a copy of the default input.txt

    QProcess m_process;

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;

    void configureProcess();
};

#endif // FRAX_MANAGER_H
