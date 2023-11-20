#include "blood_pressure_manager.h"
#include "auxiliary/json_settings.h"

#include "data/blood_pressure/tests/blood_pressure_test.h"

#include "cypress_application.h"
#include "bpm_communication.h"

#include <QCoreApplication>
#include <QDebug>
#include <QtUsb/QHidDevice>
#include <QJsonObject>
#include <QSettings>
#include <QStandardItemModel>
#include <QtUsb/QtUsb>

BloodPressureManager::BloodPressureManager(QSharedPointer<BPMSession> session)
    : ManagerBase(session)
{
    m_test.reset(new BloodPressureTest);
    m_test->setExpectedMeasurementCount(6);

    m_driver.reset(new BpTru200Driver);
    m_thread.reset(new QThread);

    m_driver->moveToThread(m_thread.get());


    connect(this, &BloodPressureManager::writeMessage, m_driver.get(), &BpTru200Driver::writeMessage);
    connect(m_driver.get(), &BpTru200Driver::receiveMessages, this, &BloodPressureManager::receiveMessages);
}

BloodPressureManager::~BloodPressureManager()
{
}

void BloodPressureManager::start()
{

}

void BloodPressureManager::receiveMessages(QList<BPMMessage> messages)
{
    while (!messages.empty())
    {
        BPMMessage message = messages.front();

        switch (message.getType())
        {
        case BPMMessage::MessageType::ACK:
            handleAck(message);
            break;
        case BPMMessage::MessageType::NACK:
            handleNack(message);
            break;
        case BPMMessage::MessageType::BUTTON:
            handleButton(message);
            break;
        case BPMMessage::MessageType::DATA:
            handleData(message);
            break;
        case BPMMessage::MessageType::NOTIFICATION:
            handleNotification(message);
            break;

        // should not appear in read messages
        case BPMMessage::MessageType::CMD:
            break;
        case BPMMessage::MessageType::UNKNOWN:
            break;
        }

        messages.pop_front();
    }
}


bool BloodPressureManager::isInstalled()
{
  return false;
}


void BloodPressureManager::setCuffSize(const QString &size)
{
    //if(size.isNull() || 0 == size.length()) return;

    //BloodPressureTest* test = static_cast<BloodPressureTest*>(m_test.get());
    //if(size.toLower() != m_cuffSize)
    //{
    //    m_cuffSize = size.toLower();
    //    test->setCuffSize(m_cuffSize);

    //    emit cuffSizeChanged(m_cuffSize);
    //}
}

void BloodPressureManager::setSide(const QString &side)
{
    //if(side.isNull() || 0 == side.length()) return;

    //BloodPressureTest* test = static_cast<BloodPressureTest*>(m_test.get());
    //if(side.toLower() != m_side)
    //{
    //    m_side = side.toLower();
    //    test->setSide(m_side);

    //    emit sideChanged(m_side);
    //}
}

// slot for UI communication
//
void BloodPressureManager::measure()
{

}


// slot for UI communication
//
void BloodPressureManager::finish()
{

}

// Set up device
bool BloodPressureManager::setUp()
{
    return true;
}

// Clean up the device for next time
bool BloodPressureManager::cleanUp()
{
    return true;
}

void BloodPressureManager::handleAck(const BPMMessage& message)
{
    // get ack type

    quint8 ackType = message.
}

void BloodPressureManager::handleNack(const BPMMessage& message)
{
    // get nack type
}

void BloodPressureManager::handleButton(const BPMMessage& message)
{
    // get button type
}

void BloodPressureManager::handleData(const BPMMessage& message)
{
    // get data type
}

void BloodPressureManager::handleNotification(const BPMMessage& message)
{
    // get notification type
}

void BloodPressureManager::handleNoMessage(const BPMMessage& message)
{
    // get no message type
}

// Clean up the device for next time
bool BloodPressureManager::clearData()
{
    return true;
}
