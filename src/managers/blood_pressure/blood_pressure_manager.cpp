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

const quint16 BloodPressureManager::vid{ 0x10b7 };
const quint16 BloodPressureManager::pid{ 0x1234 };

BloodPressureManager::BloodPressureManager(QSharedPointer<BPMSession> session)
    : ManagerBase(session)
{
    m_test.reset(new BloodPressureTest);
    m_test->setExpectedMeasurementCount(6);

    m_bpm200 = new QHidDevice(this);
    m_driver = new BpTru200Driver(m_mutex, m_bpm200, this);

    qDebug() << "BloodPressure";

    qDebug() << session->getSessionId();
    qDebug() << session->getBarcode();
    qDebug() << session->getInterviewer();
    qDebug() << session->getInputData();

    connect(m_driver, &BpTru200Driver::messagesReceived, this, &BloodPressureManager::receiveMessages);
    //connect(m_driver, &BpTru200Driver::finished, this, &BpTru200Driver::deleteLater);
}


BloodPressureManager::~BloodPressureManager()
{
    qDebug() << "destroy blood pressure manager";

    if (m_driver != nullptr && m_driver->isRunning()) {
        qDebug() << "requesting driver interrupt";
        m_driver->requestInterruption();
        qDebug() << "waiting...";
        m_driver->wait();
        qDebug() << "finished waiting...";
    }

    if (m_driver != nullptr) {
        qDebug() << "delete driver...";
        delete m_driver;
    }

    if (m_bpm200 != nullptr) {
        qDebug() << "delete hid...";
        delete m_bpm200;
    }
}

bool BloodPressureManager::isInstalled()
{
    return true;
}


void BloodPressureManager::receiveMessages(QQueue<BPMMessage> messages)
{
    if (m_sim)
        return;

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
        qDebug() << "connected to bpm";
        qDebug() << m_bpm200->manufacturer();

        m_driver->start();
        handshake();

        emit deviceConnected();
    } else {
        QMessageBox::warning(nullptr, "Error", "Could not connect to the blood pressure device");
    }
}

bool BloodPressureManager::start()
{
    qDebug() << "BloodPressureManager::start";

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

    qDebug() << "request interruption";
    m_driver->requestInterruption();
    qDebug() << "wait";
    m_driver->wait();
    qDebug() << "close";
    m_bpm200->close();
    qDebug() << "disconnected";
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
    qDebug() << "BloodPressureManager::cycle - sending cycle command";

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
    qDebug() << "BloodPressureManager::clear - sending clear command";

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
    qDebug() << "BloodPressureManager::start - sending start command";

    m_mutex.lock();
    m_driver->writeQueue.enqueue(start);
    m_mutex.unlock();
}

void BloodPressureManager::stopMeasurement()
{
    qDebug() << "BloodPressureManager::stopMeasurement - sending stop command";
    if (m_sim) {
        BPMMessage fakeStopMessage;
        onDeviceStopped(fakeStopMessage);
        return;
    }

    BPMMessage stop(0x11, 0x01, 0x00, 0x00, 0x00);

    m_mutex.lock();
    m_driver->writeQueue.enqueue(stop);
    m_mutex.unlock();
}


void BloodPressureManager::onDeviceHandshaked(const BPMMessage &message)
{
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

    qDebug() << "BloodPressureManager::deviceReset";
}

void BloodPressureManager::onDeviceCycled(const BPMMessage &message)
{
    qDebug() << "BloodPressureManager::deviceCycled";

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

    qDebug() << "BPMManager::onDeviceStarted";

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
    qDebug() << "BloodPressureManager::onDeviceStopped";

    if (m_state == State::MEASURING) {
        m_state = State::READY;
        emit deviceStateChanged("Ready");
    }

    auto test = qSharedPointerCast<BloodPressureTest>(m_test);
    test->updateAverage();

    emit measurementStopped();
}

void BloodPressureManager::onInflateCuffPressure(const BPMMessage &message)
{
    qDebug() << "BloodPressureManager::onInflateCuffPressure";

    const int ls = message.getData0() & 0x000000FF;
    const int ms = (message.getData1() << 8) & 0x0000FF00;
    const int pressure = ls + ms;

    m_measurementStartTime = QDateTime::currentDateTime();

    emit inflateCuffPressure(pressure);
}

void BloodPressureManager::onDeflateCuffPressure(const BPMMessage &message)
{
    qDebug() << "BloodPressureManager::onDeflateCuffPressure";

    const int ls = message.getData0() & 0x000000FF;
    const int ms = (message.getData1() << 8) & 0x0000FF00;
    const int pressure = ls + ms;

    emit deflateCuffPressure(pressure);
}

void BloodPressureManager::onBpResult(const BPMMessage &message)
{
    qDebug() << "BloodPressureManager::onBpResult";

    const int sbp = message.getData1();
    const int dbp = message.getData2();
    const int pulse = message.getData3();

    auto test = qSharedPointerCast<BloodPressureTest>(m_test);
    QSharedPointer<BloodPressureMeasurement> measure(new BloodPressureMeasurement);

    measure->setAttribute("reading_number", m_test->getMeasurementCount() + 1);
    measure->setAttribute("systolic",       BloodPressureMeasurement::Value(sbp, "mmHg"));
    measure->setAttribute("diastolic",      BloodPressureMeasurement::Value(dbp, "mmHg"));
    measure->setAttribute("pulse",          BloodPressureMeasurement::Value(pulse, "bpm"));
    measure->setAttribute("start_time",     m_measurementStartTime.toString(Qt::DateFormat::TextDate));
    measure->setAttribute("end_time",       QDateTime::currentDateTime().toString(Qt::DateFormat::TextDate));

    test->addMeasurement(measure);
    emit dataChanged(m_test);
}

void BloodPressureManager::onBpAverage(const BPMMessage &message)
{
    qDebug() << "BloodPressureManager::onBpAverage: avg = " << message.getData0();

    const int sbp = message.getData1();
    const int dbp = message.getData2();
    const int pulse = message.getData3();

    auto test = qSharedPointerCast<BloodPressureTest>(m_test);
    test->addDeviceAverage(sbp, dbp, pulse);

    emit dataChanged(m_test);

    if (test->isValid())
        emit canFinish();
}

void BloodPressureManager::onBpReview(const BPMMessage &message)
{
    qDebug() << "BloodPressureManager::onBpReview";

    if (message.getData0() == 0) {
        qDebug() << "computing average";
        const int sbp = message.getData1();
        const int dbp = message.getData2();
        const int pulse = message.getData3();

        auto test = qSharedPointerCast<BloodPressureTest>(m_test);
        test->addDeviceAverage(sbp, dbp, pulse);

        emit dataChanged(m_test);

        if (test->isValid())
            emit canFinish();
    }
}


void BloodPressureManager::setCuffSize(const QString &size)
{
    qDebug() << "BloodPressureManager::setCuffSize size =" << size;

    if(size.isNull() || size.isEmpty())
        return;

    auto test = qSharedPointerCast<BloodPressureTest>(m_test);
    test->setCuffSize(size);
}

void BloodPressureManager::setSide(const QString &side)
{
    qDebug() << "BloodPressureManager::setSide side = " << side;

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

void BloodPressureManager::finish() {
    auto test = qSharedPointerCast<BloodPressureTest>(m_test);
    test->updateAverage();

    ManagerBase::finish();
}

void BloodPressureManager::addManualEntry(const int systolic, const int diastolic, const int pulse)
{
    auto test = qSharedPointerCast<BloodPressureTest>(m_test);
    QSharedPointer<BloodPressureMeasurement> bpm(
        new BloodPressureMeasurement(m_test->getMeasurementCount() + 1,
                                     systolic,
                                     diastolic,
                                     pulse,
                                     QDateTime::currentDateTimeUtc(),
                                     QDateTime::currentDateTimeUtc()));
    test->addMeasurement(bpm);
    test->updateAverage();

    qDebug() << test->toJsonObject();

    emit dataChanged(m_test);
}

void BloodPressureManager::removeMeasurement(const int index)
{
    m_test->removeMeasurement(index);
    emit dataChanged(m_test);

    m_test->isValid() ? emit canFinish() : emit cannotFinish();
}

// Set up device
bool BloodPressureManager::setUp()
{
    qInfo() << "BloodPressureManager::setUp";
    return true;
}

// Clean up the device for next time
bool BloodPressureManager::cleanUp()
{
    qInfo() << "BloodPressureManager::cleanUp";
    return true;
}

void BloodPressureManager::handleAck(const BPMMessage& message)
{
    qInfo() << "BloodPressureManager::handleAck messageId = " << message.getMessageId();

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
    qInfo() << "BloodPressureManager::handleNack: " << message.getMessageId();
}

void BloodPressureManager::handleButton(const BPMMessage& message)
{
    // get button type
    qInfo() << "BloodPressureManager::handleButton: " << message.getMessageId();

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
    qInfo() << "BloodPressureManager::handleData: " << message.getMessageId();

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
    qInfo() << "BloodPressureManager::handleNotification: " << message.getMessageId();
}

void BloodPressureManager::handleNoMessage(const BPMMessage& message)
{
    // get no message type
    qInfo() << "BloodPressureManager::handleNoMessage: " << message.getMessageId();
}

// Clean up the device for next time
bool BloodPressureManager::clearData()
{
    qInfo() << "BloodPressureManager::clearData";

    m_test->reset();
    emit dataChanged(m_test);

    return true;
}
