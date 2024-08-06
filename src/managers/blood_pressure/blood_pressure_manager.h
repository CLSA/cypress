#ifndef BLOOD_PRESSURE_MANAGER_H
#define BLOOD_PRESSURE_MANAGER_H

#include "managers/manager_base.h"
#include "server/sessions/bpm_session.h"

#include "bptru_200_driver.h"

#include <QObject>
#include <QThread>
#include <QMutex>


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

public:
    enum State {
        DISCONNECTED,
        CONNECTED,
        CYCLING,
        READY,
        MEASURING,
        STOPPED,
        COMPLETE
    };

    explicit BloodPressureManager(QSharedPointer<BPMSession> session);
    ~BloodPressureManager();

    static bool isDefined();
    static bool isInstalled();

    void setCuffSize(const QString&);
    void setSide(const QString&);

    static const quint16 vid;
    static const quint16 pid;

signals:
    void canConnect();

    void deviceConnected();
    void deviceDisconnected();

    void deviceCycled(const quint8 newCycle);
    void deviceCleared();

    void measurementStarted();
    void measurementStopped();

    void deviceStateChanged(const BloodPressureManager::State state);
    void cuffPressureChanged(const int cuffPressure);

    void bpResult();
    void bpAverage();
    void review();

public slots:
    // what the manager does in response to the main application
    // window invoking its run method
    //
    void connectToDevice();
    void disconnectFromDevice();

    void receiveMessages(QQueue<BPMMessage> messages);

    bool start() override;

    void finish() override;

    // Commands from GUI
    //
    void cycle();
    void clear();
    void startMeasurement();
    void stopMeasurement();

    // retrieve a measurement from the device
    //
    void measure() override;

    void addManualEntry(const int systolic, const int diastolic, const int pulse);
    void removeMeasurement(const int index);

private:
    State m_state { State::DISCONNECTED };

    BpTru200Driver* m_driver;
    QHidDevice* m_bpm200;

    QDateTime m_measurementStartTime;

    QMutex m_mutex;

    // device data is separate from test data
    Measurement m_deviceData;

    // Reset the session
    bool clearData() override;

    void handshake();

    void handleAck(const BPMMessage& ackMessage);
    void handleNack(const BPMMessage& nackMessage);
    void handleButton(const BPMMessage& buttonMessage);
    void handleData(const BPMMessage& dataMessage);
    void handleNotification(const BPMMessage& notificationMessage);
    void handleNoMessage(const BPMMessage& noMessage);

    void onDeviceHandshaked(const BPMMessage& message);
    void onDeviceCleared(const BPMMessage& message);
    void onDeviceReset(const BPMMessage& message);
    void onDeviceCycled(const BPMMessage& message);
    void onDeviceStarted(const BPMMessage& message);
    void onDeviceStopped(const BPMMessage& message);

    void onInflateCuffPressure(const BPMMessage& message);
    void onDeflateCuffPressure(const BPMMessage& message);
    void onBpResult(const BPMMessage& message);
    void onBpAverage(const BPMMessage& message);
    void onBpReview(const BPMMessage& message);
};

#endif // BLOOD_PRESSURE_MANAGER_H
