#ifndef CDTT_MANAGER_H
#define CDTT_MANAGER_H

#include "cypress_session.h"
#include "managers/manager_base.h"
#include "data/cdtt/tests/cdtt_test.h"

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
    explicit CDTTManager(const CypressSession& session);
    ~CDTTManager();

    static bool isAvailable();
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

private:
    QProcess m_process;
    CDTTTest* m_test;

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

#endif // CDTT_MANAGER_H
