#include "bptru_200_driver.h"

#include "cypress_settings.h"


BpTru200Driver::BpTru200Driver(QObject *parent)
    : QObject{parent}
{
    m_bpm200.reset(new QHidDevice(this));
    m_read_buffer.reset(new QByteArray(1024, 0));

    m_debug = CypressSettings::isDebugMode();
    m_sim = CypressSettings::isSimMode();
}

BpTru200Driver::~BpTru200Driver()
{
    qDebug() << "destory bptru driver";
}

void BpTru200Driver::connectToDevice()
{
    if (m_debug)
    {
        qDebug() << "attempt to connect to device...";
    }

    if (m_bpm200->open(vid, pid))
    {
        if (m_debug)
        {
            qDebug() << "connected to bpm";
        }

        emit deviceConnected();
    }
    else
    {
        if (m_debug)
        {
            qDebug() << "could not connect to bpm...";
        }

        emit couldNotConnect();
    }
}

void BpTru200Driver::disconnectFromDevice()
{
    if (m_debug)
    {
        qDebug() << "attempt to disconnect from device";
    }

    if (m_bpm200->isOpen())
    {
        m_bpm200->close();

        if (m_debug)
        {
            qDebug() << "bpm disconnected";
        }
    }

    emit deviceDisconnected();
}


void BpTru200Driver::write(BPMMessage message)
{
    if (message.isValidCRC())
    {
        if (m_debug)
        {
            qDebug() << "writing message";
        }

        write(message);
    }
    else
    {
        if (m_debug)
        {
            qDebug() << "error: bpm message does not have a valid CRC";
        }

        return;
    }

    QByteArray packedMessage;

    packedMessage.append(reportNumber);
    packedMessage.append(STX);
    packedMessage.append(message.getMessageId());
    packedMessage.append(message.getData0());
    packedMessage.append(message.getData1());
    packedMessage.append(message.getData2());
    packedMessage.append(message.getData3());
    packedMessage.append(message.getCrc());
    packedMessage.append(ETX);

    qint32 bytesWritten = m_bpm200->write(&packedMessage, packedMessage.size());
    if (m_debug)
    {
        qDebug() << "BpTru200Driver::write - wrote " << bytesWritten;
    }
}

void BpTru200Driver::read()
{
    if (m_debug)
    {
        qDebug() << "BpTru200Driver - read";
    }

    if (!m_bpm200->isOpen())
    {
        qDebug() << "Tried to read before device was connected..";
        return;
    }

    // reset buffer
    for (int i = 0; i < 1024; i++)
    {
        m_read_buffer.get()[i] = 0;
    }

    // read into buffer
    quint32 bytesRead = m_bpm200->read(m_read_buffer.get(), 1024);
    if (bytesRead <= 0)
    {
        if (m_debug)
        {
            qDebug() << "bytes read <= 0, not parsing anything";
        }

        return;
    }
    else {
        if (m_debug)
        {
            qDebug() << "bytes read: " << bytesRead;
        }
    }

    // convert bytes into bytes
    if (m_debug)
    {
        qDebug() << "convert bytes into bytes";
    }

    QByteArray bytes = QByteArray::fromHex(m_read_buffer.get()[0].toHex());
    for (quint32 i = 1; i < bytesRead; i++)
    {
        QByteArray tempBytes = QByteArray::fromHex(m_read_buffer.get()[i].toHex());
        bytes.append(tempBytes);
    }

    if (m_debug)
    {
        qDebug() << "parse bytes into message";
    }

    // parse bytes into bpm messages
    parseData(bytes, bytesRead);

    if (m_debug)
    {
        qDebug() << "send to manager";
    }

    // pop from the read queue into a list and send to the manager
    QList<BPMMessage> messages;
    while (!readQueue.empty())
    {
        BPMMessage message = readQueue.front();
        readQueue.pop();

        messages.append(message);
    }

    emit receiveMessages(messages);
}

void BpTru200Driver::parseData(const QByteArray& data, quint32 bytesRead)
{
    for (quint32 i = 0; i < bytesRead; i++)
    {
        if (data[i] == STX && (i + 7) < bytesRead)
        {
            if (data[i + 7] == ETX)
            {
                qDebug() << "found message, parsing..";
                quint8 messageId = data[++i];
                quint8 data0 = data[++i];
                quint8 data1 = data[++i];
                quint8 data2 = data[++i];
                quint8 data3 = data[++i];
                quint8 crc   = data[++i];

                BPMMessage message(messageId, data0, data1, data2, data3, crc);
                if (message.isValidCRC())
                {
                    if (m_debug)
                    {
                        qDebug() << "parsed valid message";
                    }
                    readQueue.push(message);
                }
                else
                {
                    if (m_debug)
                    {
                        qDebug() << "error: message is not valid";
                    }
                }
            }
        }
    }
}


