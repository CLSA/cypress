#ifndef CDTTMANAGER_H
#define CDTTMANAGER_H

#include "managers/ManagerBase.h"
#include "data/cdtt/tests/CDTTTest.h"

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
    explicit CDTTManager(QWidget* parent = Q_NULLPTR);
    ~CDTTManager();

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

private:
    QProcess m_process;
    CDTTTest m_test;

    void clearData() override;

    void configureProcess();
};

#endif // CDTTMANAGER_H
