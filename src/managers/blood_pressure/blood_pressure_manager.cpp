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


BloodPressureManager::BloodPressureManager(QSharedPointer<BPMSession> session)
    : ManagerBase(session)
{
    m_test.reset(new BloodPressureTest);
    m_test->setExpectedMeasurementCount(6);

    m_bpm200 = new QHidDevice(this);
    m_driver = new BpTru200Driver(m_mutex, m_bpm200, this);

    qInfo() << "BloodPressureManager::BloodPressureManager";
    qInfo() << session->getSessionId();
    qInfo() << session->getBarcode();
    qInfo() << session->getInterviewer();
    qInfo() << session->getInputData();

    connect(m_driver, &BpTru200Driver::messagesReceived, this, &BloodPressureManager::receiveMessages);
}


BloodPressureManager::~BloodPressureManager()
{
    qInfo() << "BloodPressureManager::~BloodPressureManager";

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

void BloodPressureManager::receiveMessages(QQueue<BPMMessage> messages)
{
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
    qInfo() << "BloodPressureManager::connectToDevice";
    qDebug() << "attempt to connect to device...";

    if (m_bpm200->isOpen())
    {
        qDebug() << "device already open";
        emit deviceStateChanged(State::CONNECTED);
        return;
    }

    if (m_bpm200->open(vid, pid)) {
        qDebug() << "connected to bpm";
        qDebug() << m_bpm200->manufacturer();

        m_driver->start();
        handshake();
    } else {
        QMessageBox::warning(nullptr, "Error", "Could not connect to the blood pressure device");
    }
}

bool BloodPressureManager::start()
{
    qInfo() << "BloodPressureManager::start";

    emit started(m_test);
    emit canConnect();
    emit dataChanged(m_test);

    return true;
}

void BloodPressureManager::handshake()
{
    // send handshake
    BPMMessage handshake(0x11, 0x00, 0x00, 0x00, 0x00);

    m_mutex.lock();
    m_driver->writeQueue.enqueue(handshake);
    m_mutex.unlock();
}

void BloodPressureManager::disconnectFromDevice()
{
    qInfo() << "BloodPressureManager::disconnectFromDevice";

    qDebug( ) << "request interruption";
    m_driver->requestInterruption();

    qDebug() << "wait";
    m_driver->wait();

    qDebug() << "close";
    m_bpm200->close();

    qDebug() << "disconnected";
    emit deviceStateChanged(State::DISCONNECTED);
}

void BloodPressureManager::cycle()
{
    qInfo() << "BloodPressureManager::cycle";
    BPMMessage cycle(0x11, 0x03, 0x00, 0x00, 0x00);

    emit deviceStateChanged(State::CYCLING);

    m_mutex.lock();
    m_driver->writeQueue.enqueue(cycle);
    m_mutex.unlock();
}

void BloodPressureManager::clear()
{
    qInfo() << "BloodPressureManager::clear";
    BPMMessage clear(0x11, 0x05, 0x00, 0x00, 0x00);

    m_mutex.lock();
    m_driver->writeQueue.enqueue(clear);
    m_mutex.unlock();
}

void BloodPressureManager::startMeasurement()
{
    qInfo() << "BloodPressureManager::start";
    BPMMessage start(0x11, 0x04, 0x00, 0x00, 0x00);

    m_mutex.lock();
    m_driver->writeQueue.enqueue(start);
    m_mutex.unlock();
}

void BloodPressureManager::stopMeasurement()
{
    qInfo() << "BloodPressureManager::stopMeasurement";
    BPMMessage stop(0x11, 0x01, 0x00, 0x00, 0x00);

    m_mutex.lock();
    m_driver->writeQueue.enqueue(stop);
    m_mutex.unlock();
}


void BloodPressureManager::onDeviceHandshaked(const BPMMessage &message)
{
    Q_UNUSED(message)

    qDebug() << "BPMManager::deviceHandshaked";
    emit deviceStateChanged(State::CONNECTED);

    clear();
}

void BloodPressureManager::onDeviceCleared(const BPMMessage &message)
{
    Q_UNUSED(message);

    qDebug() << "BPMManager::deviceCleared";

    emit deviceCleared();

    clearData();
    cycle();
}

void BloodPressureManager::onDeviceReset(const BPMMessage &message)
{
    Q_UNUSED(message);

    qInfo() << "BloodPressureManager::deviceReset";
}

void BloodPressureManager::onDeviceCycled(const BPMMessage &message)
{
    Q_UNUSED(message)

    qInfo() << "BloodPressureManager::deviceCycled" << message.getData1();

    quint8 cycleTime = message.getData1();

    emit deviceCycled(cycleTime);

    if (cycleTime != 1)
        cycle();
    else {
        emit deviceStateChanged(State::READY);
    }
}

void BloodPressureManager::onDeviceStarted(const BPMMessage &message)
{
    Q_UNUSED(message);
    qInfo() << "BloodPressureManager::onDeviceStarted";

    auto test = qSharedPointerCast<BloodPressureTest>(m_test);

    emit deviceStateChanged(State::MEASURING);
    emit measurementStarted();
}

void BloodPressureManager::onDeviceStopped(const BPMMessage &message)
{
    Q_UNUSED(message)
    qInfo() << "BloodPressureManager::onDeviceStopped";

    auto test = qSharedPointerCast<BloodPressureTest>(m_test);
    test->updateAverage();

    emit deviceStateChanged(State::STOPPED);
    emit measurementStopped();
}

void BloodPressureManager::onInflateCuffPressure(const BPMMessage &message)
{
    const int ls = message.getData0() & 0x000000FF;
    const int ms = (message.getData1() << 8) & 0x0000FF00;
    const int pressure = ls + ms;

    m_measurementStartTime = QDateTime::currentDateTime();

    emit cuffPressureChanged(pressure);
}

void BloodPressureManager::onDeflateCuffPressure(const BPMMessage &message)
{
    const int ls = message.getData0() & 0x000000FF;
    const int ms = (message.getData1() << 8) & 0x0000FF00;
    const int pressure = ls + ms;

    emit cuffPressureChanged(pressure);
}

void BloodPressureManager::onBpResult(const BPMMessage &message)
{
    qInfo() << "BloodPressureManager::onBpResult";

    const quint8 code = message.getData0();
    const quint8 sbp = message.getData1();
    const quint8 dbp = message.getData2();
    const quint8 pulse = message.getData3();

    if (message.hasError(code)) {
        if (message.hasSbpError(code)) {
            qWarning() << "BloodPressureManager::onBpResult - Systolic error";
            QMessageBox::warning(
                nullptr,
                "Systolic error",
                message.getPulseErrorMessage(message.getSbpError(code)));
        }
        else if (message.hasDbpError(code)) {
            qWarning() << "BloodPressureManager::onBpResult - Diastolic error";
            QMessageBox::warning(
                nullptr,
                "Diastolic error",
                message.getPulseErrorMessage(message.getDbpError(code)));
        }
        else if (message.hasPulseError(code)) {
            qWarning() << "BloodPressureManager::onBpResult - Pulse error";
            QMessageBox::warning(
                nullptr,
                "Pulse error",
                message.getPulseErrorMessage(message.getPulseError(code)));
        }
        else if (message.hasSystemError(code)) {
            qWarning() << "BloodPressureManager::onBpResult - System error";
            QMessageBox::warning(
                nullptr,
                "System error",
                message.getSystemErrorMessage(message.getSystemError(code)));
        }
        else {
            qWarning() << "BloodPressureManager::onBpResult - unknown error type";
            QMessageBox::critical(
                nullptr,
                "Unknown error",
                "An unexpected error occurred");
        }
        return;
    }

    auto test = qSharedPointerCast<BloodPressureTest>(m_test);
    QSharedPointer<BloodPressureMeasurement> measure(new BloodPressureMeasurement);

    const int readingNumber = m_test->getMeasurementCount() + 1;

    measure->setAttribute("reading_number", readingNumber);
    measure->setAttribute("systolic",       BloodPressureMeasurement::Value(sbp, "mmHg"));
    measure->setAttribute("diastolic",      BloodPressureMeasurement::Value(dbp, "mmHg"));
    measure->setAttribute("pulse",          BloodPressureMeasurement::Value(pulse, "bpm"));
    measure->setAttribute("start_time",     m_measurementStartTime.toString(Qt::DateFormat::TextDate));
    measure->setAttribute("end_time",       QDateTime::currentDateTime().toString(Qt::DateFormat::TextDate));

    test->addMeasurement(measure);
    test->updateAverage();

    emit dataChanged(m_test);
    if (m_test->isValid()) {
        emit canFinish();
    }
    else {
        emit cannotFinish();
    }
}

void BloodPressureManager::onBpAverage(const BPMMessage &message)
{
    qInfo() << "BloodPressureManager::onBpAverage";
    const int sbp = message.getData1();
    const int dbp = message.getData2();
    const int pulse = message.getData3();

    auto test = qSharedPointerCast<BloodPressureTest>(m_test);
    test->addDeviceAverage(sbp, dbp, pulse);
    emit dataChanged(m_test);

    if (m_test->isValid()) {
        emit canFinish();
        emit deviceStateChanged(State::COMPLETE);
    }
    else {
        emit cannotFinish();
    }
}

void BloodPressureManager::onBpReview(const BPMMessage &message)
{
    qInfo() << "BloodPressureManager::onBpReview";

    if (message.getData0() != 0)
        return;

    const int sbp = message.getData1();
    const int dbp = message.getData2();
    const int pulse = message.getData3();

    auto test = qSharedPointerCast<BloodPressureTest>(m_test);
    test->addDeviceAverage(sbp, dbp, pulse);

    emit dataChanged(m_test);

    if (m_test->isValid()) {
        emit canFinish();
        emit deviceStateChanged(State::COMPLETE);
    }
    else {
        emit cannotFinish();
    }
}


void BloodPressureManager::setCuffSize(const QString &size)
{
    qInfo() << "BloodPressureManager::setCuffSize:" << size;

    if(size.isNull() || size.isEmpty())
        return;

    auto test = qSharedPointerCast<BloodPressureTest>(m_test);
    test->setCuffSize(size);
}

void BloodPressureManager::setSide(const QString &side)
{
    qInfo() << "BloodPressureManager::setSide:" << side;

    if (side.isNull() || side.isEmpty())
        return;

    auto test = qSharedPointerCast<BloodPressureTest>(m_test);
    test->setSide(side);
}

// slot for UI communication
//
void BloodPressureManager::measure()
{
    qInfo() << "BloodPressureManager::measure";
}

void BloodPressureManager::finish() {
    qInfo() << "BloodPressureManager::finish";

    auto test = qSharedPointerCast<BloodPressureTest>(m_test);
    test->updateAverage();

    if (test->getMeasurementCount() < 2) {
        qWarning() << "BloodPressureManager::finish - invalid test, this should not occur";
        m_test->reset();
        QMessageBox::critical(nullptr, "Unexpected error", "An unexpected error occurred, please use manual entry or try again");
        return;
    }

    ManagerBase::finish();
}

void BloodPressureManager::addManualEntry(const int systolic, const int diastolic, const int pulse)
{
    qInfo() << "BloodPressureManager::addManualEntry";

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

    if (m_test->isValid()) {
        emit canFinish();
    }
    else {
        emit cannotFinish();
    }
}

void BloodPressureManager::removeMeasurement(const int index)
{
    qInfo() << "BloodPressureManager::removeMeasurement: " << index;
    auto test = qSharedPointerCast<BloodPressureTest>(m_test);

    test->removeMeasurement(index);
    test->updateAverage();

    emit dataChanged(m_test);
    if (m_test->isValid()) {
        emit canFinish();
    }
    else {
        emit cannotFinish();
    }
}

void BloodPressureManager::handleAck(const BPMMessage& message)
{
    qDebug() << "BloodPressureManager::handleAck messageId = " << message.getMessageId();

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
    qDebug() << "BloodPressureManager::handleNack: " << message.getMessageId();
}

void BloodPressureManager::handleButton(const BPMMessage& message)
{
    // get button type
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
    Q_UNUSED(message)
    // get notification type
    qInfo() << "BloodPressureManager::handleNotification";
}

void BloodPressureManager::handleNoMessage(const BPMMessage& message)
{
    Q_UNUSED(message)
    // get no message type
    qInfo() << "BloodPressureManager::handleNoMessage";
}

// Clean up the device for next time
bool BloodPressureManager::clearData()
{
    qInfo() << "BloodPressureManager::clearData";

    const QString cuffSize = m_test->getMetaDataAsString("cuff_size");
    const QString armUsed = m_test->getMetaDataAsString("arm_used");

    m_test->reset();

    m_test->addMetaData("cuff_size", cuffSize);
    m_test->addMetaData("arm_used", armUsed);

    emit dataChanged(m_test);

    return true;
}
