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
    m_thread.reset(new QThread);

    m_driver->moveToThread(m_thread.get());

    connect(m_thread.get(), &QThread::started, m_driver.get(), &BpTru200Driver::connectToDevice);
    connect(m_thread.get(), &QThread::finished, m_driver.get(), &BpTru200Driver::disconnectFromDevice);

    connect(m_driver.get(), &BpTru200Driver::couldNotConnect, this, [=]() {
        if (m_debug)
        {
            qDebug() << "BloodPressureManager::couldNotConnect slot: could not connect to device";
        }

        QMessageBox::warning(nullptr, "Could not connect to BPM", "");
    });

    connect(m_driver.get(), &BpTru200Driver::deviceConnected, this, [=]() {
        BPMMessage handshake(0x11, 0x00, 0x00, 0x00, 0x00);

        if (m_debug)
        {
            qDebug() << "BloodPressureManager::deviceConnected -- sending handshake message";
        }

        // Write handshake message
        emit writeMessage(handshake);
        emit readMessages();
    });

    connect(this, &BloodPressureManager::writeMessage, m_driver.get(), &BpTru200Driver::write);
    connect(this, &BloodPressureManager::readMessages, m_driver.get(), &BpTru200Driver::read);

    connect(m_driver.get(), &BpTru200Driver::receiveMessages, this, &BloodPressureManager::receiveMessages);
}

BloodPressureManager::~BloodPressureManager()
{
    if (m_debug)
    {
        qDebug() << "destroy blood pressure manager";
    }

    m_thread->terminate();
}

void BloodPressureManager::start()
{
    if (m_debug)
    {
        qDebug() << "BloodPressureManager::start";
    }

    m_thread->start();
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
