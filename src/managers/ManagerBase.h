#ifndef MANAGERBASE_H
#define MANAGERBASE_H

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

    // Is this instrument available at the installed workstation?
    static bool isDefined();

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
    void canWrite();

protected:
    // The unique identifier representing this test session (for pine state management)
    QString m_uuid;

    // Context dependent clear test data and possibly device data (eg., serial port info)
    // SerialPortManager class clears device data during setDevice() while
    // test data is cleared depending on derived class implementation requirements.
    // Derived classes may also clear test data depending on the nature of the test,
    // such as when multiple measurements are separately acquired.
    //

    // Set up device
    virtual bool setUp() = 0;

    // Reset the session
    virtual bool clearData() = 0;

    // Clean up the device for next time
    virtual bool cleanUp() = 0;

    // Send the results to Pine for storage & analysis
    virtual bool sendResultsToPine(const QJsonObject &data) = 0;
};

#endif // MANAGERBASE_H
