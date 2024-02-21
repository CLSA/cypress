#include "managers/blood_pressure/blood_pressure_manager.h"
#include "data/blood_pressure/tests/blood_pressure_test.h"
#include "data/blood_pressure/measurements/blood_pressure_measurement.h"

#include <QtUsb/QtUsb>

#include <QRandomGenerator>
#include <QCoreApplication>
#include <QDebug>
#include <QJsonObject>
#include <QMessageBox>
#include <QSettings>
#include <QStandardItemModel>
#include <QtUsb/QHidDevice>

const quint16 BloodPressureManager::vid{0x10b7};
const quint16 BloodPressureManager::pid{0x1234};

BloodPressureManager::BloodPressureManager(QSharedPointer<BPMSession> session)
    : ManagerBase(session)
{
    m_test.reset(new BloodPressureTest);
    m_test->setExpectedMeasurementCount(6);

    m_bpm200.reset(new QHidDevice(this));
    m_driver.reset(new BpTru200Driver(m_mutex, m_bpm200));

    if (m_debug) {
        qDebug() << "BloodPressure";

        qDebug() << session->getSessionId();
        qDebug() << session->getBarcode();
        qDebug() << session->getInterviewer();
        qDebug() << session->getInputData();
    }

    connect(m_driver.get(), &BpTru200Driver::messagesReceived, this, &BloodPressureManager::receiveMessages);
}


BloodPressureManager::~BloodPressureManager()
{
    if (m_debug)
        qDebug() << "destroy blood pressure manager";
}

bool BloodPressureManager::isInstalled()
{
    bool isDebugMode{CypressSettings::isDebugMode()};
    bool isSimMode{CypressSettings::isSimMode()};

    if (isSimMode) {
        return true;
    }

    QHidDevice device;
    if (device.open(BloodPressureManager::vid, BloodPressureManager::pid)) {
        device.close();
        return true;
    } else {
        if (isDebugMode)
            qDebug() << "BloodPressureManager::isInstalled - could not open bpm device";
    }

    return false;
}


void BloodPressureManager::receiveMessages(QQueue<BPMMessage> messages)
{
    if (m_sim) {
        return;
    }

    if (m_debug)
        qDebug() << "BloodPressureManager::received " << messages.length() << " messages";

    while (!messages.empty())
    {
        BPMMessage message = messages.dequeue();

        switch (message.getType())
        {
        case BPMMessage::MessageType::Ack:
            handleAck(message);
            break;

        case BPMMessage::MessageType::Nack:
            handleNack(message);
            break;

        case BPMMessage::MessageType::Button:
            handleButton(message);
            break;

        case BPMMessage::MessageType::Data:
            handleData(message);
            break;

        case BPMMessage::MessageType::Notification:
            handleNotification(message);
            break;

        case BPMMessage::MessageType::Command:
        case BPMMessage::Reset:
        case BPMMessage::MessageType::Unknown:
            handleData(message);
            break;
        }
    }
}

void BloodPressureManager::connectToDevice()
{
    if (m_debug)
        qDebug() << "attempt to connect to device...";

    if (m_sim) {
        handshake();
        emit deviceConnected();

        return;
    }

    if (m_bpm200->isOpen())
    {
        qDebug() << "device already open";
        emit deviceConnected();

        return;
    }

    if (m_bpm200->open(vid, pid)) {
        if (m_debug)
        {
            qDebug() << "connected to bpm";
            qDebug() << m_bpm200->manufacturer();

        }

        m_driver->start();
        handshake();

        emit deviceConnected();
    } else {
        QMessageBox::warning(nullptr, "Error", "Could not connect to the blood pressure device");
    }
}

bool BloodPressureManager::start()
{
    if (m_debug) {
        qDebug() << "BloodPressureManager::start";
    }

    if (m_sim) {
        qDebug() << "canConect";
        emit canMeasure();
        emit canConnect();

        return true;
    }

    emit started(m_test);
    emit canConnect();
    emit dataChanged(m_test);

    return true;
}

void BloodPressureManager::handshake()
{
    // send handshake
    BPMMessage handshake(0x11, 0x00, 0x00, 0x00, 0x00);
    if (m_sim) {
        BPMMessage fakeHandshake;
        onDeviceHandshaked(fakeHandshake);
        return;
    }

    m_mutex.lock();
    m_driver->writeQueue.enqueue(handshake);
    m_mutex.unlock();
}

void BloodPressureManager::disconnectFromDevice()
{
    if (m_sim) {
        emit deviceDisconnected();
        return;
    }

    m_driver->requestInterruption();
    while (!m_driver->isFinished())
    {
        QThread::sleep(300);
    }

    m_bpm200->disconnect();

    emit deviceDisconnected();
}

void BloodPressureManager::cycle()
{
    if (m_sim) {
        BPMMessage fakeCycleMessage;
        onDeviceCycled(fakeCycleMessage);
        return;
    }

    BPMMessage cycle(0x11, 0x03, 0x00, 0x00, 0x00);
    if (m_debug) {
        qDebug() << "BloodPressureManager::cycle - sending cycle command";
    }

    m_mutex.lock();
    m_driver->writeQueue.enqueue(cycle);
    m_mutex.unlock();
}

void BloodPressureManager::clear()
{
    if (m_sim) {
        BPMMessage fakeClearMessage;
        onDeviceCleared(fakeClearMessage);
        return;
    }

    // clear device
    BPMMessage clear(0x11, 0x05, 0x00, 0x00, 0x00);
    if (m_debug) {
        qDebug() << "BloodPressureManager::clear - sending clear command";
    }

    m_mutex.lock();
    m_driver->writeQueue.enqueue(clear);
    m_mutex.unlock();
}

void BloodPressureManager::startMeasurement()
{
    if (m_sim) {
        qDebug() << "start measurement";
        BPMMessage fakeStartMessage;
        onDeviceStarted(fakeStartMessage);
        return;
    }

    BPMMessage start(0x11, 0x04, 0x00, 0x00, 0x00);
    if (m_debug) {
        qDebug() << "BloodPressureManager::start - sending start command";
    }

    m_mutex.lock();
    m_driver->writeQueue.enqueue(start);
    m_mutex.unlock();
}

void BloodPressureManager::stopMeasurement()
{
    if (m_sim) {
        BPMMessage fakeStopMessage;
        onDeviceStopped(fakeStopMessage);
        return;
    }

    BPMMessage stop(0x11, 0x01, 0x00, 0x00, 0x00);
    if (m_debug) {
        qDebug() << "BloodPressureManager::start - sending stop command";
    }

    m_mutex.lock();
    m_driver->writeQueue.enqueue(stop);
    m_mutex.unlock();
}




void BloodPressureManager::onDeviceHandshaked(const BPMMessage &message)
{
    if (m_debug)
        qDebug() << "BPMManager::deviceHandshaked";

    if (m_sim) {
        emit deviceHandshaked("SIM");

        return;
    }

    if (m_state == State::CONNECTING) {
        QString firmwareVersion = QString("%1.%2").arg(message.getData1(),
                                                       message.getData2() * 10 + message.getData3());
        emit deviceHandshaked(firmwareVersion);

        clear();
    }
}

void BloodPressureManager::onDeviceCleared(const BPMMessage &message)
{
    Q_UNUSED(message);

    if (m_debug)
        qDebug() << "BPMManager::deviceCleared";

    if (m_state == State::CONNECTING) {
        cycle();
    } else if (m_state == State::READY) {
        //start();
    } else if (m_state == State::MEASURING) {
        m_state = State::READY;
        emit deviceStateChanged("Ready");
    }

    emit deviceCleared();
}

void BloodPressureManager::onDeviceReset(const BPMMessage &message)
{
    Q_UNUSED(message);

    if (m_debug)
        qDebug() << "BPMManager::deviceReset";
}

void BloodPressureManager::onDeviceCycled(const BPMMessage &message)
{
    if (m_debug)
        qDebug() << "BPMManager::deviceCycled";

    static quint8 cycleTime = 0;

    if (m_sim) {
        QThread::msleep(300);
        cycleTime = (cycleTime + 1) % 4;
    }
    else {
        cycleTime = message.getData1();
    }

    emit deviceCycled(QString::number(cycleTime));

    if (m_state == State::CONNECTING) {
        if (cycleTime != 1)
            cycle();
        else {
            m_state = State::READY;
            emit deviceStateChanged("Ready");
        }
    } else if (m_state == State::READY) {
        if (cycleTime != 1)
            cycle();
    }
}

void BloodPressureManager::onDeviceStarted(const BPMMessage &message)
{
    Q_UNUSED(message);

    if (m_debug)
        qDebug() << "BPMManager::deviceStarted";

    if (m_state == State::READY) {
        m_state = State::MEASURING;

        auto test = qSharedPointerCast<BloodPressureTest>(m_test);

        emit deviceStateChanged("Measuring");
        emit measurementStarted();

        if (m_sim) {

            QRandomGenerator *generator = QRandomGenerator::global();

            int sbpTotal = 0;
            int dbpTotal = 0;
            int pulseTotal = 0;

            double sbpAvg = 0.0;
            double dbpAvg = 0.0;
            double pulseAvg = 0.0;

            for (int i = 0; i < 4; i++) {

                int sbp = generator->bounded(80, 121);
                int dbp = generator->bounded(80, 121);
                int pulse = generator->bounded(40, 100);

                QSharedPointer<BloodPressureMeasurement> measure(new BloodPressureMeasurement);
                measure->setAttribute("reading_number", i + 1);
                measure->setAttribute("start_time", QDateTime::currentDateTime().toString());
                measure->setAttribute("end_time", QDateTime::currentDateTime().toString());

                measure->setAttribute("systolic", BloodPressureMeasurement::Value(sbp, "mmHg"));
                measure->setAttribute("diastolic", BloodPressureMeasurement::Value(dbp, "mmHg"));
                measure->setAttribute("pulse", BloodPressureMeasurement::Value(pulse, "bpm"));

                test->addMeasurement(measure);

                if (i == 0) { // skip first measure
                    continue;
                }

                sbpTotal += sbp;
                dbpTotal += dbp;
                pulseTotal += pulse;
            }

            sbpAvg = sbpTotal / 3.0;
            dbpAvg = dbpTotal / 3.0;
            pulseAvg = pulseTotal / 3.0;

            test->addDeviceAverage(qRound(sbpAvg), qRound(dbpAvg), qRound(pulseAvg));
            emit dataChanged(m_test);
            return;
        }
    }

    emit measurementStarted();
}

void BloodPressureManager::onDeviceStopped(const BPMMessage &message)
{
    Q_UNUSED(message)

    if (m_debug)
        qDebug() << "BPMManager::deviceStopped";

    if (m_state == State::MEASURING) {
        m_state = State::READY;
        emit deviceStateChanged("Ready");
    }

    emit measurementStopped();
}

void BloodPressureManager::onInflateCuffPressure(const BPMMessage &message)
{
    if (m_debug)
        qDebug() << "BloodPressureManager::onInflateCuffPressure";

    int ls = message.getData0() & 0x000000FF;
    int ms = (message.getData1() << 8) & 0x0000FF00;
    int pressure = ls + ms;

    m_measurementStartTime = QDateTime::currentDateTime();

    emit inflateCuffPressure(pressure);
}

void BloodPressureManager::onDeflateCuffPressure(const BPMMessage &message)
{
    if (m_debug)
        qDebug() << "BloodPressureManager::onDeflateCuffPressure";

    int ls = message.getData0() & 0x000000FF;
    int ms = (message.getData1() << 8) & 0x0000FF00;
    int pressure = ls + ms;

    emit deflateCuffPressure(pressure);
}

void BloodPressureManager::onBpResult(const BPMMessage &message)
{
    if (m_debug)
        qDebug() << "BloodPressureManager::onBpResult";

    int sbp = message.getData1();
    int dbp = message.getData2();
    int pulse = message.getData3();

    QSharedPointer<BloodPressureMeasurement> measure(new BloodPressureMeasurement);
    measure->setAttribute("reading_number", m_test->getMeasurementCount() + 1);
    measure->setAttribute("systolic", BloodPressureMeasurement::Value(sbp, "mmHg"));
    measure->setAttribute("diastolic", BloodPressureMeasurement::Value(dbp, "mmHg"));
    measure->setAttribute("pulse", BloodPressureMeasurement::Value(pulse, "bpm"));
    measure->setAttribute("start_time", m_measurementStartTime.toString(Qt::DateFormat::TextDate));
    measure->setAttribute("end_time", QDateTime::currentDateTime().toString(Qt::DateFormat::TextDate));

    BloodPressureTest* test = static_cast<BloodPressureTest*>(m_test.get());
    test->addMeasurement(measure);


    emit dataChanged(m_test);
}

void BloodPressureManager::onBpAverage(const BPMMessage &message)
{
    if (m_debug)
        qDebug() << "bp average: " << message.getData0();

    int sbp = message.getData1();
    int dbp = message.getData2();
    int pulse = message.getData3();

    BloodPressureTest* test = static_cast<BloodPressureTest*>(m_test.get());
    test->addDeviceAverage(sbp, dbp, pulse);

    emit dataChanged(m_test);

    if (test->isValid()) {
        emit canFinish();
    }
}

void BloodPressureManager::onBpReview(const BPMMessage &message)
{
    if (m_debug)
        qDebug() << "bp review";

    if (message.getData0() == 0) {
        qDebug() << "computing average";
        int sbp = message.getData1();
        int dbp = message.getData2();
        int pulse = message.getData3();

        BloodPressureTest* test = static_cast<BloodPressureTest*>(m_test.get());
        test->addDeviceAverage(sbp, dbp, pulse);

        emit dataChanged(m_test);

        if (test->isValid()) {
            emit canFinish();
        }
    }
}


void BloodPressureManager::setCuffSize(const QString &size)
{
    if (m_debug)
        qDebug() << "BloodPressureManager::setCuffSize" << size;

    if(size.isNull() || size.isEmpty())
        return;

    auto test = qSharedPointerCast<BloodPressureTest>(m_test);
    test->setCuffSize(size);
}

void BloodPressureManager::setSide(const QString &side)
{
    if (m_debug)
        qDebug() << "BloodPressureManager::setSide" << side;

    if (side.isNull() || side.isEmpty())
        return;

    auto test = qSharedPointerCast<BloodPressureTest>(m_test);
    test->setSide(side);
}

// slot for UI communication
//
void BloodPressureManager::measure()
{
}

void BloodPressureManager::addManualMeasurement()
{
    QSharedPointer<BloodPressureMeasurement> bpm(
        new BloodPressureMeasurement(m_test->getMeasurementCount() + 1,
                                     0,
                                     0,
                                     0,
                                     QDateTime::currentDateTimeUtc(),
                                     QDateTime::currentDateTimeUtc()));
    m_test->addMeasurement(bpm);

    emit dataChanged(m_test);
}

// Set up device
bool BloodPressureManager::setUp()
{
    if (m_debug)
        qDebug() << "BloodPressureManager::setUp";

    return true;
}

// Clean up the device for next time
bool BloodPressureManager::cleanUp()
{
    if (m_debug)
        qDebug() << "BloodPressureManager::cleanUp";

    return true;
}

void BloodPressureManager::handleAck(const BPMMessage& message)
{
    if (m_debug)
        qDebug() << "BloodPressureManager::handleAck" << message.getMessageId();

    // get ack type
    quint8 ackType = message.getData0();

    switch (ackType)
    {
        case BPMMessage::AckType::AckHandshake:
            onDeviceHandshaked(message);
            break;
        case BPMMessage::AckType::AckClear:
            onDeviceCleared(message);
            break;
        case BPMMessage::AckType::AckCycle:
            onDeviceCycled(message);
            break;
        case BPMMessage::AckType::AckStart:
            onDeviceStarted(message);
            break;
        case BPMMessage::AckType::AckStop:
            onDeviceStopped(message);
            break;
        case BPMMessage::AckType::AckReview:
            break;
    }
}

void BloodPressureManager::handleNack(const BPMMessage& message)
{
    // get nack type
    if (m_debug)
        qDebug() << "BloodPressureManager::handleNack: " << message.getMessageId();
}

void BloodPressureManager::handleButton(const BPMMessage& message)
{
    // get button type
    if (m_debug)
        qDebug() << "BloodPressureManager::handleButton: " << message.getMessageId();

    quint8 buttonType = message.getData0();
    switch(buttonType)
    {
        case BPMMessage::ButtonType::StartButton:
            onDeviceStarted(message);
            break;
        case BPMMessage::ButtonType::StopButton:
            onDeviceStopped(message);
            break;
        case BPMMessage::ButtonType::ClearButton:
            onDeviceCleared(message);
            break;
        case BPMMessage::ButtonType::CycleButton:
            onDeviceCycled(message);
            break;
        case BPMMessage::ButtonType::ReviewButton:
            break;
    }
}

void BloodPressureManager::handleData(const BPMMessage& message)
{
    // get data type
    if (m_debug)
        qDebug() << "BloodPressureManager::handleData: " << message.getMessageId();

    quint8 messageId = message.getMessageId();

    switch (messageId)
    {
        case BPMMessage::DataType::BpDeflCuffPressure:
            onDeflateCuffPressure(message);
            break;
        case BPMMessage::DataType::BpInflCuffPressure:
            onInflateCuffPressure(message);
            break;
        case BPMMessage::DataType::BpAvg:
            onBpAverage(message);
            break;
        case BPMMessage::DataType::BpResult:
            onBpResult(message);
            break;
        case BPMMessage::DataType::BpReview:
            onBpReview(message);
            break;
    }
}

void BloodPressureManager::handleNotification(const BPMMessage& message)
{
    // get notification type
    if (m_debug)
        qDebug() << "BloodPressureManager::handleNotification: " << message.getMessageId();
}

void BloodPressureManager::handleNoMessage(const BPMMessage& message)
{
    // get no message type
    if (m_debug)
        qDebug() << "BloodPressureManager::handleNoMessage: " << message.getMessageId();
}

// Clean up the device for next time
bool BloodPressureManager::clearData()
{
    if (m_debug)
        qDebug() << "BloodPressureManager::clearData";

    m_test->reset();
    emit dataChanged(m_test);

    return true;
}
