#include "blood_pressure_manager.h"
#include "data/blood_pressure/tests/blood_pressure_test.h"

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
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::start";
    }
}

void BloodPressureManager::receiveMessages(QList<BPMMessage> messages)
{
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::receiveMessages";
    }

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
            if (m_debug)
            {
                qDebug() << "invalid: receiveMessage received CMD message from device";
            }

            break;

        case BPMMessage::MessageType::UNKNOWN:
            if (m_debug)
            {
                qDebug() << "invalid: unknown message received from device";
            }

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
}


// slot for UI communication
//
void BloodPressureManager::finish()
{
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::finish";
    }
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
    //quint8 ackType = message.
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
}

void BloodPressureManager::handleData(const BPMMessage& message)
{
    // get data type
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::handleData: " << message.getMessageId();
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

    return true;
}
