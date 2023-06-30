#ifndef MANAGER_BASE_H
#define MANAGER_BASE_H

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

class ManagerBase : public QObject
{
    Q_OBJECT

public:
    explicit ManagerBase();
    ~ManagerBase();

    bool sendCancellation(QString device, QString uuid);
    bool sendComplete(QString device, QString uuid);


    // The unique identifier representing this test session (for pine state management)
    QString m_uuid;

    // The ID to sent the response to (Pine)
    qint32 m_answerId;

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

signals:

    // the underlying test data has changed
    //
    void dataChanged();

    // ready to measure and receive data
    // (update GUI enable measure button)
    //
    void canMeasure();

    // valid test completed and ready to write to output
    // (update GUI enable write button and update the results display)
    //
    void canFinish();

protected:
    // Input data from Pine
    QString m_barcode { "" };
    QString m_interviewer { "" };
    QVariantMap m_inputData;
    QList<QString> m_inputKeyList;

    QVariant getInputDataValue(const QString &);
    QVariantMap jsonObjectToVariantMap(const QJsonObject& jsonObject);

    virtual void sendHTTPRequest(const QString& method, const QString& endpoint, const QString& contentType, const QByteArray& data);
    virtual void sendHTTPSRequest(const QString& method, const QString& endpoint, const QString& contentType, const QByteArray& data);

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

    // Send the test results to Pine for storage & analysis
    virtual bool sendResultsToPine(const QString& filePath);
    virtual bool sendFileToPine(const QString& filePath, const QString& fileName);

    virtual void sendJsonData(const QString& filePath);
    virtual void sendOctetStream(const QString& filePath);
};

#endif // MANAGER_BASE_H
