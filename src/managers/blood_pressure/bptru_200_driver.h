#ifndef BPTRU200DRIVER_H
#define BPTRU200DRIVER_H

#include <QObject>
#include <QtUsb/QHidDevice>

#include <queue>

class BPMMessage {
public:
    enum MessageType {
        CMD = 0x11,
        RESET = 0x10,
        ACK = 0x10,
        NACK = 0x15,
        BUTTON = 0x55,
        NOTIFICATION = 0x21,
        DATA,
        UNKNOWN
    };

    enum AckType {
        HANDSHAKE = 0x00,
        STOP = 0x01,
        //REVIEW = 0x02,
        CYCLE = 0x03,
        START = 0x04,
        CLEAR = 0x05,
    };

    enum ButtonType {
        STOPPED = 0x01,
        //REVIEW = 0x02,
        CYCLED = 0x03,
        STARTED = 0x04,
        CLEARED = 0x05,
    };

    enum DataType {
        INFL_CUFF_PRESSURE = 0x49,
        DEFL_CUFF_PRESSURE = 0x44,
        BP_RESULT = 0x42,
        BP_AVG = 0x41,
        REVIEW = 0x52,
    };

    BPMMessage(
        quint8 messageId,
        quint8 data0,
        quint8 data1,
        quint8 data2,
        quint8 data3,
        quint8 crc
        ): m_messageId(messageId), m_data0(data0), m_data1(data1), m_data2(data2), m_data3(data3), m_crc(crc) {

          };

    BPMMessage::MessageType getType() {
        switch (m_messageId)
        {
        case MessageType::ACK:
            return MessageType::ACK;
        case MessageType::NACK:
            return MessageType::NACK;
        case MessageType::BUTTON:
            return MessageType::BUTTON;
        case MessageType::DATA:
            return MessageType::DATA;
        case MessageType::NOTIFICATION:
            return MessageType::NOTIFICATION;
        }

        return MessageType::UNKNOWN;
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
    quint8 m_messageId{};
    quint8 m_data0{};
    quint8 m_data1{};
    quint8 m_data2{};
    quint8 m_data3{};
    quint8 m_crc{};
};

struct BPMError {

};

class BpTru200Driver : public QObject
{
    Q_OBJECT

public:
    explicit BpTru200Driver(QObject *parent = nullptr);
    ~BpTru200Driver();

signals:
    void receiveMessages(QList<BPMMessage> messages);

public slots:
    void writeMessage(BPMMessage message);

private:
    void read();
    void write(BPMMessage);

    bool connect();
    bool disconnect();

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
