#include "blood_pressure_manager.h"
#include "../../data/blood_pressure/tests/blood_pressure_test.h"
#include "auxiliary/CRC8.h"

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

    m_driver.reset(new BpTru200Driver);
}

BloodPressureManager::~BloodPressureManager()
{
    if (m_debug)
    {
        qDebug() << "destroy blood pressure manager";
    }
}

void BloodPressureManager::start()
{
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::start";
    }

    if (!m_driver->connectToDevice())
    {
        qCritical() << "error couldn't connect to device";
    }

    // send handshake
    BPMMessage handshake(0x11, 0x00, 0x00, 0x00, 0x00);
    if (!m_driver->write(handshake))
    {
        qCritical() << "couldn't write handshake message";
    }
    if (!m_driver->read())
    {
        qCritical() << "couldn't read handshake ack";
    }

    // clear device
    BPMMessage clear(0x11, 0x05, 0x00, 0x00, 0x00);
    if (!m_driver->write(clear))
    {
        qCritical() << "couldn't write clear message";
    }
    if (!m_driver->read())
    {
        qCritical() << "couldn't read clear ack";
    }

    // set cycle to 1
    BPMMessage cycle(0x11, 0x03, 0x00, 0x00, 0x00);
    if (!m_driver->write(cycle))
    {
        qCritical() << "couldn't write cycle message";
    }
    if (!m_driver->read())
    {
        qCritical() << "couldn't read cycle ack";
    }

    for (int i = 0; i < m_driver->readQueue.size(); i++)
    {
        qDebug() << m_driver->readQueue.front().toString();
        m_driver->readQueue.pop();
    }
}

void BloodPressureManager::receiveMessages(QList<BPMMessage> messages)
{
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::received " << messages.length() << " messages";
    }

    while (!messages.empty())
    {
        BPMMessage message = messages.front();

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

        // should not appear in read messages
        case BPMMessage::MessageType::Command:
            if (m_debug)
            {
                qDebug() << "invalid: receiveMessage received CMD message from device";
            }

            break;

        case BPMMessage::MessageType::Unknown:
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
    quint8 ackType = message.getMessageId();
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
