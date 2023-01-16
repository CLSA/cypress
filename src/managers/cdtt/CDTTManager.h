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

    // is the passed string a jar file
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

    // set the executable full path and name
    // calls isDefined to validate the passed arg
    //
    void selectRunnable(const QString&);

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
    QString m_runnableName;// full pathspec to CDTTstereo.jar
    QString m_runnablePath;// path to CDTTstereo.jar

    QString m_outputPath;     // path to output .csv files
    QString m_outputFile;     // full pathspec to working output .csv file
    QProcess m_process;

    CDTTTest m_test;

    void clearData() override;

    void configureProcess();
};

#endif // CDTTMANAGER_H
