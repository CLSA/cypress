#include "blood_pressure_manager.h"

#include "data/blood_pressure/tests/blood_pressure_test.h"

#include <QMessageBox>
#include <QCoreApplication>
#include <QDebug>
#include <QtUsb/QHidDevice>
#include <QJsonObject>
#include <QSettings>
#include <QStandardItemModel>
#include <QtUsb/QtUsb>


#include <QMessageBox>

BloodPressureManager::BloodPressureManager(QSharedPointer<BPMSession> session)
    : ManagerBase(session)
{
    m_test.reset(new BloodPressureTest);
    m_test->setExpectedMeasurementCount(6);

    m_bpm200.reset(new QHidDevice(this));
    m_driver.reset(new BpTru200Driver(m_mutex, m_bpm200));
}


BloodPressureManager::~BloodPressureManager()
{
    if (m_debug)
    {
        qDebug() << "destroy blood pressure manager";
    }
}

void BloodPressureManager::connectToDevice()
{
    if (m_debug)
    {
        qDebug() << "attempt to connect to device...";
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

void BloodPressureManager::start()
{
    if (m_debug) {
        qDebug() << "BloodPressureManager::start";
    }

    if (m_sim) {
        emit canMeasure();
    }

    emit started(m_test.get());
    emit dataChanged(m_test.get());
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
    BPMMessage cycle(0x11, 0x03, 0x00, 0x00, 0x00);

    m_mutex.lock();
    m_driver->writeQueue.enqueue(cycle);
    m_mutex.unlock();
}

void BloodPressureManager::clear()
{
    // clear device
    BPMMessage clear(0x11, 0x05, 0x00, 0x00, 0x00);

    m_mutex.lock();
    m_driver->writeQueue.enqueue(clear);
    m_mutex.unlock();
}

void BloodPressureManager::startMeasurement()
{
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::start";
    }

    BPMMessage start(0x11, 0x04, 0x00, 0x00, 0x00);

    m_mutex.lock();
    m_driver->writeQueue.enqueue(start);
    m_mutex.unlock();
}

void BloodPressureManager::stopMeasurement()
{
    if (m_debug) {
        qDebug() << "BloodPressureManager::start";
    }

    BPMMessage stop(0x11, 0x01, 0x00, 0x00, 0x00);

    m_mutex.lock();
    m_driver->writeQueue.enqueue(stop);
    m_mutex.unlock();
}

void BloodPressureManager::receiveMessages(QQueue<BPMMessage> messages)
{
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::received " << messages.length() << " messages";
    }

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

        messages.pop_front();
    }
}

bool BloodPressureManager::isInstalled()
{
    return true;
}

void BloodPressureManager::onDeviceHandshaked(const BPMMessage &message)
{
    if (m_debug)
    {
        qDebug() << "BPMManager::deviceHandshaked";
    }

    QString firmwareVersion = QString("%1.%2").arg(message.getData1(), message.getData2() * 10 + message.getData3());
    emit deviceHandshaked(firmwareVersion);
}

void BloodPressureManager::onDeviceCleared(const BPMMessage &message)
{
    if (m_debug)
    {
        qDebug() << "BPMManager::deviceCleared";
    }

    clearData();

    emit deviceCleared();
}

void BloodPressureManager::onDeviceReset(const BPMMessage &message)
{
    if (m_debug)
    {
        qDebug() << "BPMManager::deviceReset";
    }
}

void BloodPressureManager::onDeviceCycled(const BPMMessage &message)
{
    if (m_debug)
    {
        qDebug() << "BPMManager::deviceCycled";
    }

    quint8 cycleTime = message.getData1();

    emit deviceCycled(QString::number(cycleTime));
}

void BloodPressureManager::onDeviceStarted(const BPMMessage &message)
{
    if (m_debug)
    {
        qDebug() << "BPMManager::deviceStarted";
    }

    emit measurementStarted();
}

void BloodPressureManager::onDeviceStopped(const BPMMessage &message)
{
    if (m_debug)
    {
        qDebug() << "BPMManager::deviceStopped";
    }

    emit measurementStopped();
}

void BloodPressureManager::onDeviceData(const BPMMessage &message)
{

}

void BloodPressureManager::onInflateCuffPressure(const BPMMessage &message)
{
    if (m_debug)
    {
        qDebug() << "inflate cuff pressure";
    }
}

void BloodPressureManager::onDeflateCuffPressure(const BPMMessage &message)
{
    if (m_debug)
    {
        qDebug() << "deflate cuff pressure";
    }
}

void BloodPressureManager::onBpResult(const BPMMessage &message)
{
    if (m_debug)
    {
        qDebug() << "bp result";
    }

    int sbp = message.getData1();
    int dbp = message.getData2();
    int pulse = message.getData3();

    QSharedPointer<BloodPressureMeasurement> measure(new BloodPressureMeasurement);
    measure->setAttribute("systolic", BloodPressureMeasurement::Value(sbp, "mmHg"));
    measure->setAttribute("diastolic", BloodPressureMeasurement::Value(dbp, "mmHg"));
    measure->setAttribute("pulse", BloodPressureMeasurement::Value(pulse, "bpm"));

    BloodPressureTest* test = static_cast<BloodPressureTest*>(m_test.get());
    test->addMeasurement(measure);
}

void BloodPressureManager::onBpAverage(const BPMMessage &message)
{
    if (m_debug)
    {
        qDebug() << "bp average";
        qDebug() << "count: " << message.getData0();
    }

    int sbp = message.getData1();
    int dbp = message.getData2();
    int pulse = message.getData3();

    BloodPressureTest* test = static_cast<BloodPressureTest*>(m_test.get());
    test->addDeviceAverage(sbp, dbp, pulse);
}

void BloodPressureManager::onBpReview(const BPMMessage &message)
{
    if (m_debug)
    {
        qDebug() << "bp review";
    }
}


void BloodPressureManager::setCuffSize(const QString &size)
{
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::setCuffSize";
    }

    if(size.isNull() || size.isEmpty())
    {
        if (m_debug)
        {
            qDebug() << "cuff size is not valid: " << size;
        }

        return;
    }

    BloodPressureTest* test = static_cast<BloodPressureTest*>(m_test.get());
    test->setCuffSize(size);
}

void BloodPressureManager::setSide(const QString &side)
{
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::setSide";
    }

    if (side.isNull() || side.isEmpty())
    {
        if (m_debug)
        {
            qDebug() << "side is not valid: " << side;
        }

        return;
    }

    BloodPressureTest* test = static_cast<BloodPressureTest*>(m_test.get());
    test->setSide(side);
}

// slot for UI communication
//
void BloodPressureManager::measure()
{
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::measure";
    }

    if (m_sim) {
        m_test->reset();
        m_test->simulate();

        emit dataChanged(m_test.get());
    }
}

// slot for UI communication
//
void BloodPressureManager::finish()
{
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::finish";
    }

    disconnectFromDevice();
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

    emit dataChanged(m_test.get());
}

// Set up device
bool BloodPressureManager::setUp()
{
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::setUp";
    }

    return true;
}

// Clean up the device for next time
bool BloodPressureManager::cleanUp()
{
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::cleanUp";
    }

    return true;
}

void BloodPressureManager::handleAck(const BPMMessage& message)
{
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::handleAck" << message.getMessageId();
    }

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
    {
        qDebug() << "BloodPressureManager::handleNack: " << message.getMessageId();
    }
}

void BloodPressureManager::handleButton(const BPMMessage& message)
{
    // get button type
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::handleButton: " << message.getMessageId();
    }

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
    {
        qDebug() << "BloodPressureManager::handleData: " << message.getMessageId();
    }

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
    {
        qDebug() << "BloodPressureManager::handleNotification: " << message.getMessageId();
    }
}

void BloodPressureManager::handleNoMessage(const BPMMessage& message)
{
    // get no message type
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::handleNoMessage: " << message.getMessageId();
    }
}

// Clean up the device for next time
bool BloodPressureManager::clearData()
{
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::clearData";
    }

    m_test->reset();

    return true;
}
