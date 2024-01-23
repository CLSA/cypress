#ifndef MANAGER_BASE_H
#define MANAGER_BASE_H

#include "cypress_settings.h"
#include "data/test_base.h"

#include <QJsonObject>
#include <QObject>
#include <QWidget>

/*!
 * \class ManagerBase
 * \brief The base Manager class
 *
 * Abstract parent class for all device managers.
 * Manager classes are the mediators for CLI and UI interactions
 * with physical devices and/or executable programs.  Manager
 * classes control device selection, connectivity, operation,
 * test subject biographical injection and test data retrieval.
 *
 * \sa SerialPortManager, BloodPressureManager, CDTTManager, AudiometerManager
 *
 */

QT_FORWARD_DECLARE_CLASS(QSettings)
QT_FORWARD_DECLARE_CLASS(QStandardItemModel)

class CypressSession;

class ManagerBase : public QObject
{
    Q_OBJECT

public:
    explicit ManagerBase(QSharedPointer<CypressSession> session);
    ~ManagerBase();

    bool sendCancellation(QString uuid);
    bool sendComplete(QString uuid);

    void setManualEntry(bool isManualEntry);

public slots:
    // subclasses call methods after main initialization just prior
    // to running (eg., emit dataChanged signal)
    //
    virtual bool start() = 0;

    // actual measure will only execute if the barcode has been
    // verified.  Subclasses must reimplement accordingly.
    //
    virtual void measure() = 0;

    // subclasses call methods just prior to main close event
    //
    virtual void finish();

    // Request from the view to add a manual measurement
    virtual void addManualMeasurement();

protected:

    // Set up device
    virtual bool setUp() = 0;

    // Reset the session
    virtual bool clearData() = 0;

    // Clean up the device for next time
    virtual bool cleanUp() = 0;

    QSharedPointer<CypressSession> m_session;
    QSharedPointer<TestBase> m_test;

    QVariantMap m_inputData;
    QList<QString> m_inputKeyList;

    bool m_debug { true };
    bool m_sim { true };

    bool manualEntryMode { false };

    QVariant getInputDataValue(const QString &);


signals:

    // the underlying test data has changed
    //
    void dataChanged(QSharedPointer<TestBase> test);

    // initialize UI with basic test info
    void started(QSharedPointer<TestBase> test);

    // ready to measure and receive data
    // (update GUI enable measure button)
    //
    void canMeasure();
    void cannotMeasure();

    // valid test completed and ready to write to output
    // (update GUI enable write button and update the results display)
    //
    void canFinish();

    // Session successful, data sent to server
    void success(const QString& successMsg);

    // Something went wrong, critical error, couldn't save data
    void error(const QString& errorMsg);

private:
    // backupData
    // restoreData
    // restoreDirectoriesAndFiles
    // restor

};

#endif // MANAGER_BASE_H
