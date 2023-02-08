#ifndef BLOODPRESSUREMANAGER_H
#define BLOODPRESSUREMANAGER_H

#include "managers/ManagerBase.h"
#include "data/blood_pressure/tests/BloodPressureTest.h"

#include <QObject>
#include <QThread>
#include <QtUsb/QtUsb>

/*!
 * \class BloodPressureManager
 * \brief HID Device manager class for BpTru BPM200
 *
 * Concrete child class implementation of a USB human interface device (HID)
 * manager. This class facilitates connection to a BpTru BPM200 blood pressure
 * monitor for read/write instructions and test data retrieval.
 *
 * HID devices are identified and selected by product and vendor ID pairs
 * (ie., QUsb::Id).  Signals and slots are implemented for UI selection from a
 * list of discovered devices and for running a blood pressure test.
 * Communication with the device is facilitated by a worker class on a
 * separate thread with signal and slot connections between worker and manager.
 * The default vendor ID for BpTru can be overridden.
 *
 * \sa ManagerBase, BPMCommunication, BPMMessage
 *
 */

QT_FORWARD_DECLARE_CLASS(BPMCommunication)

class BloodPressureManager : public ManagerBase
{
    Q_OBJECT

    Q_PROPERTY(QString deviceName MEMBER m_deviceName NOTIFY deviceNameChanged)
    Q_PROPERTY(QString cuffSize MEMBER m_cuffSize NOTIFY cuffSizeChanged)
    Q_PROPERTY(QString side MEMBER m_side NOTIFY sideChanged)

public:
    explicit BloodPressureManager();
    ~BloodPressureManager();

    const quint16 BPTRU_VENDOR_ID { 4279 };

    static bool isDefined();

    //TODO: use cypress constant for all use of size and side
    void setCuffSize(const QString&);

    //TODO: use cypress constant for all use of size and side
    void setSide(const QString&);

    void setVendorIdFilter(const quint16& vid);

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

    bool scanDevices();

    // set the device by descritive label
    //
    void selectDevice(const QString&);
    void connectDevice();
    void disconnectDevice();

private slots:
    // slot for signals coming from BPMCommunication
    //
    void measurementAvailable(const int&, const int&, const int&, const int&,
                              const QDateTime&, const QDateTime&);
    void averageAvailable(const int&, const int&, const int&);
    void finalReviewAvailable(const int&, const int&, const int&);
    void connectionStatusChanged(const bool&);
    void abortComplete(const bool&);
    void deviceInfoAvailable();

    // set the device
    //
    void setDevice(const QUsb::Id&);

signals:
    // signals to BPMCommunication
    //
    void attemptConnection(const QUsb::Id&);
    void startMeasurement();
    void abortMeasurement(QThread*);

    void canConnectDevice();
    void scanningDevices();
    void deviceDiscovered(const QString&);
    void canSelectDevice();
    void deviceSelected(const QString&);

    void deviceNameChanged(const QString&);
    void sideChanged(const QString&);
    void cuffSizeChanged(const QString&);

private:
    BloodPressureTest m_test;

    // device data is separate from test data
    Measurement m_deviceData;

    // Set up device
    bool setUp() override;

    // Reset the session
    bool clearData() override;

    // Clean up the device for next time
    bool cleanUp() override;

    // Send the results to Pine for storage & analysis
    bool sendResultsToPine(const QJsonObject &data) override;

    bool m_aborted { false };

    // usb hid devices plugged in and openable
    QMap<QString,QUsb::Id> m_deviceList;

    // called when loading from settings
    void selectDeviceById(const QUsb::Id&);

    // the usb hid device
    QString m_deviceName;
    QUsb::Id m_device;
    quint16 m_vendorIDFilter { 0 };
    QString m_cuffSize { "" };
    QString m_side { "" };

    // communications handling
    QThread m_thread;
    BPMCommunication* m_comm { Q_NULLPTR };
};

#endif // BLOODPRESSUREMANAGER_H
