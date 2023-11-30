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

bool BpTru200Driver::connectToDevice()
{
    if (m_debug)
    {
        qDebug() << "attempt to connect to device...";
    }

    if (m_bpm200->isOpen()) {
        m_bpm200->close();
    }

    if (m_bpm200->open(vid, pid)) {
        if (m_debug)
        {
            qDebug() << "connected to bpm";
            qDebug() << m_bpm200->manufacturer();
        }

        return true;
    } else {
        if (m_debug)
        {
            qDebug() << "could not connect to bpm...";
        }

        return false;
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
}


qint32 BpTru200Driver::write(BPMMessage message)
{
    message.calculateCrc();

    qDebug() << message.isValidCRC();

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

    if (!m_bpm200->isOpen()) {
        qDebug() << "connection not open";
        return -1;
    }

    qint32 bytesWritten = m_bpm200->write(&packedMessage, packedMessage.size());
    if (m_debug)
    {
        qDebug() << "BpTru200Driver::write -" << bytesWritten << "bytes";
    }

    return bytesWritten;
}

qint32 BpTru200Driver::read(int timeoutMs)
{
    if (m_debug)
    {
        qDebug() << "BpTru200Driver - read";
    }

    if (!m_bpm200->isOpen())
    {
        qDebug() << "Tried to read before device was connected..";
        return -1;
    }

    qDebug() << "reset buffer";

    m_read_buffer.get()->fill(0x00);

    qDebug() << "buffer reset, reading";

    // read into buffer
    quint32 bytesRead = m_bpm200->read(m_read_buffer.get(), 1024, timeoutMs);
    if (bytesRead <= 0)
    {
        if (m_debug)
        {
            qDebug() << "bytes read <= 0, not parsing anything";
        }

        return -1;
    }

    if (m_debug)
    {
        qDebug() << "bytes read: " << bytesRead;
    }

    qDebug() << "bytes: " << m_read_buffer->toHex();

    QByteArray bytes = QByteArray::fromHex(m_read_buffer.get()[0].toHex());
    for (quint32 i = 1; i < bytesRead; i++) {
        QByteArray tempBytes = QByteArray::fromHex(m_read_buffer.get()[i].toHex());
        bytes.append(tempBytes);
    }

    if (m_debug)
    {
        qDebug() << "parse bytes into message";
    }

    // parse bytes into bpm messages
    parseData(bytesRead);

    return bytesRead;
}

void BpTru200Driver::parseData(quint32 bytesRead)
{
    for (quint32 i = 0; i < bytesRead; i++)
    {
        if (m_read_buffer->at(i) == STX && (i + 7) < bytesRead) {
            if (m_read_buffer->at(i + 7) == ETX) {
                qDebug() << "found message, parsing..";
                quint8 messageId = m_read_buffer->at(++i);
                quint8 data0 = m_read_buffer->at(++i);
                quint8 data1 = m_read_buffer->at(++i);
                quint8 data2 = m_read_buffer->at(++i);
                quint8 data3 = m_read_buffer->at(++i);
                quint8 crc = m_read_buffer->at(++i);

                BPMMessage message(messageId, data0, data1, data2, data3, crc);

                qDebug() << message.toString();

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


