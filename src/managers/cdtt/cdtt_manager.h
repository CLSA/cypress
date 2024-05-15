#ifndef CDTT_MANAGER_H
#define CDTT_MANAGER_H

#include "managers/manager_base.h"
#include "server/sessions/cdtt_session.h"

#include <QProcess>


/*!
 * \class CDTTManager
 * \brief The CDTTManager class
 *
 * Concrete child class implementation of a device manager.
 * This class facilitates launch of the Canadian Digit Triplet Test
 * Java .jar program (CDTTstereo.jar) and reading the test output
 * .xml files it produces.  QProcess is used to facilitate exe operations
 * along with helper class ExcelQueryHelper to read the MS Excel file.
 *
 * \sa ManagerBase, ExcelQueryHelper, ChoiceReactionManager, FraxManager
 *
 */

class CDTTManager : public ManagerBase
{
    Q_OBJECT

public:
    explicit CDTTManager(QSharedPointer<CDTTSession> session);
    ~CDTTManager();

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
    QProcess m_process;

    QString m_jre;
    QString m_runnableName;
    QString m_runnablePath;

    QString m_outputPath;
    QString m_outputFile;

    QString m_settingsFilePath;

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;

    void configureProcess();

};

#endif // CDTT_MANAGER_H
