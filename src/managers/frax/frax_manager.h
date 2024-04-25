#ifndef FRAX_MANAGER_H
#define FRAX_MANAGER_H

#include "managers/manager_base.h"
#include "server/sessions/frax_session.h"

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
    bool start() override;

    void measure() override;

    void readOutput() override;

private:
    QString m_country_code;
    QString m_type_code;

    QString m_runnableName;		// full path to blackbox.exe
    QString m_runnablePath; // path to blackbox.exe directory

    QString m_outputFilePath;    		// full path to output.txt
    QString m_inputFilePath;     		// full path to input.txt
    QString m_temporaryFilePath; 		// store a copy of the default input.txt

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
