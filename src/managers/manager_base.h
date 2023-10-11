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
    explicit ManagerBase(const CypressSession& session);
    ~ManagerBase();

    bool sendCancellation(QString uuid);
    bool sendComplete(QString uuid);

    void setManualEntry(bool isManualEntry);

public slots:
    // subclasses call methods after main initialization just prior
    // to running (eg., emit dataChanged signal)
    //
    virtual void start() = 0;

    // actual measure will only execute if the barcode has been
    // verified.  Subclasses must reimplement accordingly.
    //
    virtual void measure() = 0;

    // subclasses call methods just prior to main close event
    //
    virtual void finish() = 0;

    // Request from the view to add a manual measurement
    virtual void addManualMeasurement();

signals:

    // the underlying test data has changed
    //
    void dataChanged(TestBase *test);

    // initialize UI with basic test info
    void started(TestBase *test);

    // ready to measure and receive data
    // (update GUI enable measure button)
    //
    void canMeasure();

    // measurements read, stored in the test
    void measured(TestBase *test);

    // valid test completed and ready to write to output
    // (update GUI enable write button and update the results display)
    //
    void canFinish();

    // Session successful, data sent to server
    void success(const QString& successMsg);

    // Something went wrong, critical error, couldn't save data
    void error(const QString& errorMsg);

protected:
    const CypressSession& m_session;
    const CypressSettings& m_settings;

    QVariantMap m_inputData;
    QList<QString> m_inputKeyList;

    QScopedPointer<TestBase> m_test;

    bool manualEntryMode { false };

    QVariant getInputDataValue(const QString &);
    QVariantMap jsonObjectToVariantMap(const QJsonObject& jsonObject);

    // Context dependent clear test data and possibly device data (eg., serial port info)
    // SerialPortManager class clears device data during setDevice() while
    // test data is cleared depending on derived class implementation requirements.
    // Derived classes may also clear test data depending on the nature of the test,
    // such as when multiple measurements are separately acquired.
    //
    virtual void setInputData(const QVariantMap&) = 0;

    // Set up device
    virtual bool setUp() = 0;

    // Reset the session
    virtual bool clearData() = 0;

    // Clean up the device for next time
    virtual bool cleanUp() = 0;

    virtual void sendHTTPRequest(const QString& method, const QString& endpoint, const QString& contentType, const QByteArray& data);
    virtual void sendHTTPSRequest(const QString& method, const QString& endpoint, const QString& contentType, const QByteArray& data);

    // Send the test results to Pine for storage & analysis
    virtual bool sendResultsToPine(const QString& filePath);
    virtual bool sendFileToPine(const QString& filePath, const QString& fileName);

    virtual void sendJsonData(const QString& filePath);
    virtual void sendOctetStream(const QString& filePath);
};

#endif // MANAGER_BASE_H
