#ifndef BLOOD_PRESSURE_MANAGER_H
#define BLOOD_PRESSURE_MANAGER_H

#include "data/blood_pressure/tests/blood_pressure_test.h"
#include "managers/manager_base.h"
#include "server/sessions/bpm_session.h"

#include <QObject>
#include <QThread>

#include "bptru_200_driver.h"

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

    enum State {
        CONNECTING,
        READY,
        MEASURING,
    };

signals:
    void deviceConnected();
    void deviceDisconnected();

public:
    explicit BloodPressureManager(QSharedPointer<BPMSession> session);
    ~BloodPressureManager();

    static bool isDefined();
    static bool isInstalled();

    void setCuffSize(const QString&);
    void setSide(const QString&);

signals:
    void writeMessage(BPMMessage message);

public slots:
    void receiveMessages(QList<BPMMessage> messages);

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
    // communications handling
    QScopedPointer<QThread> m_thread;
    QScopedPointer<BpTru200Driver> m_driver;

    State m_state { State::CONNECTING };

    // device data is separate from test data
    Measurement m_deviceData;

    // Set up device
    bool setUp() override;

    // Reset the session
    bool clearData() override;

    // Clean up the device for next time
    bool cleanUp() override;

    void handleAck(const BPMMessage& ackMessage);
    void handleNack(const BPMMessage& nackMessage);
    void handleButton(const BPMMessage& buttonMessage);
    void handleData(const BPMMessage& dataMessage);
    void handleNotification(const BPMMessage& notificationMessage);
    void handleNoMessage(const BPMMessage& noMessage);
};



#endif // BLOOD_PRESSURE_MANAGER_H
