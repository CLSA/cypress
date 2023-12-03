#include "bptru_200_driver.h"

#include "cypress_settings.h"

#include <QThread>

BpTru200Driver::BpTru200Driver(QMutex& mutex, QSharedPointer<QHidDevice> bpm200, QObject *parent)
    : QThread{parent}, m_mutex(mutex), m_bpm200(bpm200)
{
    m_read_buffer.reset(new QByteArray(1024, 0));

    m_debug = CypressSettings::isDebugMode();
}

BpTru200Driver::~BpTru200Driver()
{
    if (m_debug)
    {
        qDebug() << "destory bptru driver";
    }
}


void BpTru200Driver::run()
{
    while (!isInterruptionRequested() && !isFinished())
    {
        m_mutex.lock();
        if (!writeQueue.empty())
        {
            // write message
            BPMMessage message = writeQueue.dequeue();
            write(message);
        }
        m_mutex.unlock();

        read(300 /* ms */);

        if (!readQueue.isEmpty())
        {
            emit messagesReceived(readQueue);
            readQueue.clear();
        }
    }
}

qint32 BpTru200Driver::write(BPMMessage message)
{
    message.calculateCrc();

    if (m_debug)
    {
        qDebug() << "BpTru200Driver:: isValidCRC: " << message.isValidCRC();
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

    m_read_buffer.get()->fill(0x00);
    if (m_debug)
    {
        qDebug() << "reset buffer, reading...";
    }

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
        qDebug() << "bytes: " << m_read_buffer->toHex();
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
                    readQueue.enqueue(message);
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


