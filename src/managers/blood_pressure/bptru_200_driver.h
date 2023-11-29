#ifndef BPTRU200DRIVER_H
#define BPTRU200DRIVER_H

#include <QObject>
#include <QtUsb/QHidDevice>

#include "auxiliary/CRC8.h"

#include <queue>

class BPMMessage: public QObject {
    Q_OBJECT
public:
    enum MessageType {
        Command = 0x11,
        Reset = 0x10,
        Ack = 0x10,
        Nack = 0x15,
        Button = 0x55,
        Notification = 0x21,
        Data,
        Unknown
    };

    enum AckType {
        AckHandshake = 0x00,
        AckStop = 0x01,
        AckReview = 0x02,
        AckCycle = 0x03,
        AckStart = 0x04,
        AckClear = 0x05,
    };

    enum ButtonType {
        StopButton = 0x01,
        ReviewButton = 0x02,
        CycleButton = 0x03,
        StartButton = 0x04,
        ClearButton = 0x05,
    };

    enum DataType {
        BpInflCuffPressure = 0x49,
        BpDeflCuffPressure = 0x44,
        BpResult = 0x42,
        BpAvg = 0x41,
        BpReview = 0x52,
    };

    BPMMessage(
        quint8 messageId,
        quint8 data0,
        quint8 data1,
        quint8 data2,
        quint8 data3,
        quint8 crc = 0x00
        ): m_messageId(messageId), m_data0(data0), m_data1(data1), m_data2(data2), m_data3(data3), m_crc(crc) {
            if (!crc)
            {
                calculateCrc();
            }
        };

    BPMMessage(const BPMMessage& other)
    {
        m_messageId = other.getMessageId();
        m_data0 = other.getData0();
        m_data1 = other.getData1();
        m_data2 = other.getData2();
        m_data3 = other.getData3();
        m_crc = other.getCrc();
    }

    BPMMessage(BPMMessage&& other)
    {
        m_messageId = other.getMessageId();
        m_data0 = other.getData0();
        m_data1 = other.getData1();
        m_data2 = other.getData2();
        m_data3 = other.getData3();
        m_crc = other.getCrc();
    }


    BPMMessage()
    {
        m_messageId = 0x00;
        m_data0 = 0x00;
        m_data1 = 0x00;
        m_data2 = 0x00;
        m_data3 = 0x00;
        m_crc = 0x00;
    }

    BPMMessage::MessageType getType() {
        switch (m_messageId)
        {
        case MessageType::Ack:
            return MessageType::Ack;
        case MessageType::Nack:
            return MessageType::Nack;
        case MessageType::Button:
            return MessageType::Button;
        case MessageType::Data:
            return MessageType::Data;
        case MessageType::Notification:
            return MessageType::Notification;
        }

        return MessageType::Unknown;
    };

    quint8 getMessageId() const { return m_messageId; } ;
    void setMessageId(quint8 data) { m_messageId = data; }

    quint8 getData0() const { return m_data0; };
    void setData0(quint8 data) { m_data0 = data; };

    quint8 getData1() const { return m_data1; };
    void setData1(quint8 data) { m_data1 = data; };

    quint8 getData2() const { return m_data2; };
    void setData2(quint8 data) { m_data2 = data; };

    quint8 getData3() const { return m_data3; };
    void setData3(quint8 data) { m_data3 = data; };

    quint8 getCrc() const { return m_crc; };
    void setCrc(quint8 crc) { m_crc = crc; };

    void calculateCrc() {
        m_crc = CRC8::calculate(getBytes(), getBytes().length());
    }

    bool isValidCRC() { return true; };

    QByteArray getBytes() const {
        QByteArray bytes;

        bytes.append(m_messageId);
        bytes.append(m_data0);
        bytes.append(m_data1);
        bytes.append(m_data2);
        bytes.append(m_data3);
        bytes.append(m_crc);

        return bytes;
    };

private:
    quint8 m_messageId{ 0x00 };
    quint8 m_data0{ 0x00 };
    quint8 m_data1{ 0x00 };
    quint8 m_data2{ 0x00 };
    quint8 m_data3{ 0x00 };
    quint8 m_crc{ 0x00 };
};


class BpTru200Driver : public QObject
{
    Q_OBJECT

public:
    explicit BpTru200Driver(QObject *parent = nullptr);
    ~BpTru200Driver();

signals:
    void receiveMessages(QList<BPMMessage> messages);

    void deviceDisconnected();
    void deviceConnected();

    void couldNotConnect();

public slots:
    void read();
    void write(BPMMessage message);

    void connectToDevice();
    void disconnectFromDevice();

private:
    bool m_debug;
    bool m_sim;



    QScopedPointer<QByteArray> m_read_buffer;
    QScopedPointer<QHidDevice> m_bpm200;

    quint16 vid { 0x10b7 };
    quint16 pid { 0x1234 };

    quint8 STX { 0x02 };
    quint8 ETX { 0x03 };
    quint8 reportNumber { 0x00 };

    void parseData(const QByteArray& data, quint32 bytesRead);

    std::queue<BPMMessage> writeQueue;
    std::queue<BPMMessage> readQueue;
};

#endif // BPTRU200DRIVER_H
