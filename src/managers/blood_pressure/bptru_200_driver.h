#ifndef BPTRU200DRIVER_H
#define BPTRU200DRIVER_H

#include <QThread>
#include <QObject>
#include <QQueue>
#include <QMutex>

#include <QtUsb/QHidDevice>

#include "auxiliary/CRC8.h"

#include <queue>

class BPMMessage: public QObject {
    Q_OBJECT
public:
    enum MessageType {
        Command = 0x11,
        Reset = 0x10,
        Ack = 0x06,
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

    enum ErrorType {
        Indeterminate,
        UnderRange,
        OverRange,
        ArithmiticError,
        UnknownError,
    };

    enum SystemError {
        TooFewPulses = 0x0,
        TooManyMotionArtifacts = 0x01,
        Overpressure = 0x02,
        InflationTooSlow = 0x03,
        InflationTooFast = 0x04,
        DeflationTooSlow = 0x05,
        DeflationTooFast = 0x06,
        SoftwareFault = 0x07,
        PulseAmplitudeTooLow = 0x08,
        PressureBelowMinus10 = 0x09,
        PressureTooHighForTooLong = 0x0A,
        PressureNotLowEnoughForTooLong = 0x0B,
        STAM_LTAM_Violation = 0x0C,
        ResetByMeansOtherThanPowerUp = 0x0D,
        ResetByMeansOtherThanPowerUpRecoveryDataCorrupted = 0x0E,
        BatteryTooLow = 0x0F,
        UnknownSystemError = 0xFF
    };

    BPMMessage(
        quint8 messageId, quint8 data0, quint8 data1, quint8 data2, quint8 data3, quint8 crc = 0x00)
        : m_messageId(messageId)
        , m_data0(data0)
        , m_data1(data1)
        , m_data2(data2)
        , m_data3(data3)
        , m_crc(crc){};

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

    bool hasError(const quint8 code) const {
        return hasSystemError(code) ||
               hasSbpError(code)    ||
               hasDbpError(code)    ||
               hasPulseError(code);
    }

    bool hasSystemError(const quint8 code) const {
        return (code & 0x80) == 0x80;
    }

    bool hasSbpError(const quint8 code) const {
        return (code & 0x40) == 0x40;
    }

    bool hasDbpError(const quint8 code) const {
        return (code & 0x20) == 0x20;
    }

    bool hasPulseError(const quint8 code) const {
        return (code & 0x10) == 0x10;
    }

    ErrorType getDbpError(const quint8 code) const {
        switch(code) {
            case 0x15:
                return ErrorType::Indeterminate;
            case 0x18:
                return ErrorType::UnderRange;
            case 0x20:
                return ErrorType::OverRange;
            case 0x21:
                return ErrorType::ArithmiticError;
            default:
                return ErrorType::UnknownError;
        }
    }

    ErrorType getSbpError(const quint8 code) const {
        switch (code) {
            case 0x16:
                return ErrorType::Indeterminate;
            case 0x17:
                return ErrorType::UnderRange;
            case 0x19:
                return ErrorType::OverRange;
            case 0x21:
                return ErrorType::ArithmiticError;
            default:
                return ErrorType::UnknownError;
        }
    }

    ErrorType getPulseError(const quint8 code) const {
        switch(code) {
            case 0x16:
                return ErrorType::Indeterminate;
            case 0x21:
                return ErrorType::ArithmiticError;
            default:
                return ErrorType::UnknownError;
        }
    }

    QString getPulseErrorMessage(const ErrorType type) const {
        switch (type) {
            case ErrorType::Indeterminate:
                return QString("Pulse was indeterminate");
            case ErrorType::ArithmiticError:
                return QString("Pulse arithmitic error");
            case ErrorType::OverRange:
                return QString("Pulse was over range");
            case ErrorType::UnderRange:
                return QString("Pulse was under range");
            default:
                return QString("Unknown pulse error");
        }
    }

    QString getDbpErrorMessage(const ErrorType type) const {
        switch (type) {
            case ErrorType::Indeterminate:
                return QString("Diastolic was indeterminate");
            case ErrorType::ArithmiticError:
                return QString("Diastolic arithmitic error");
            case ErrorType::OverRange:
                return QString("Diastolic was over range");
            case ErrorType::UnderRange:
                return QString("Diastolic was under range");
            default:
                return QString("Unknown diastolic error");
        }
    }

    QString getSbpErrorMessage(const ErrorType type) const {
        switch (type) {
            case ErrorType::Indeterminate:
                return QString("Systolic was indeterminate");
            case ErrorType::ArithmiticError:
                return QString("Systolic arithmitic error");
            case ErrorType::OverRange:
                return QString("Systolic was over range");
            case ErrorType::UnderRange:
                return QString("Systolic was under range");
            default:
                return QString("Unknown diastolic error");
        }
    }

    SystemError getSystemError(const quint8 code) const {
        for (int i = SystemError::TooFewPulses; i < SystemError::UnknownSystemError; i++) {
            if (i == code) {
                return static_cast<SystemError>(i);
            }
        }
        return SystemError::UnknownSystemError;
    }

    QString getSystemErrorMessage(const SystemError type) const {
        switch (type) {
            case TooFewPulses:
                return QString("Too few pulses");
            case TooManyMotionArtifacts:
                return QString("Too many motion artifacts");
            case Overpressure:
                return QString("Overpressure");
            case InflationTooSlow:
                return QString("Inflation too slow");
            case InflationTooFast:
                return QString("Inflation too fast");
            case DeflationTooSlow:
                return QString("Deflation too slow");
            case DeflationTooFast:
                return QString("Deflation too fast");
            case SoftwareFault:
                return QString("Software fault");
            case PulseAmplitudeTooLow:
                return QString("Pulse amplitude too low");
            case PressureBelowMinus10:
                return QString("Pressure below minus 10");
            case PressureTooHighForTooLong:
                return QString("Pressure too high for too long");
            case PressureNotLowEnoughForTooLong:
                return QString("Pressure not low enough for too long");
            case STAM_LTAM_Violation:
                return QString("STAM / LTAM violation");
            case ResetByMeansOtherThanPowerUp:
                return QString("Reset by means other than power up");
            case ResetByMeansOtherThanPowerUpRecoveryDataCorrupted:
                return QString("Reset by means other than power up recovery, data corrupted");
            case BatteryTooLow:
                return QString("Battery too low");
            default:
                return QString("Unknown system error");
        }
    }

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

    void calculateCrc() { m_crc = CRC8::calculate(getBytes(), getBytes().length()); }

    bool isValidCRC() { return CRC8::isValid(getBytes(), getBytes().length(), m_crc); };

    QByteArray getBytes() const {
        QByteArray bytes;

        bytes.append(m_messageId);
        bytes.append(m_data0);
        bytes.append(m_data1);
        bytes.append(m_data2);
        bytes.append(m_data3);

        return bytes;
    };

    QString toString() {
        return QString("ID: %1 D0: %2 D1: %3 D2: %4 D3: %5 CRC: %6")
            .arg(
                QString::number(getMessageId(), 16),
                QString::number(getData0(), 16),
                QString::number(getData1(), 16),
                QString::number(getData2(), 16),
                QString::number(getData3(), 16),
                QString::number(getCrc(), 16)
                );
    };

private:
    quint8 m_messageId{ 0x00 };
    quint8 m_data0{ 0x00 };
    quint8 m_data1{ 0x00 };
    quint8 m_data2{ 0x00 };
    quint8 m_data3{ 0x00 };
    quint8 m_crc{ 0x00 };
};


class BpTru200Driver : public QThread
{
    Q_OBJECT
public:
    explicit BpTru200Driver(QMutex& mutex, QHidDevice* hid, QObject *parent = nullptr);
    ~BpTru200Driver();

    qint32 write(BPMMessage message);
    qint32 read(int timeoutMs = -1); // default blocking read

    QQueue<BPMMessage> writeQueue;

    void run() override;

signals:
    void messagesReceived(QQueue<BPMMessage> messages);

private:
    QScopedPointer<QByteArray> m_read_buffer;

    // mutex is for the write queue
    QMutex& m_mutex;

    QQueue<BPMMessage> readQueue;
    QHidDevice* m_bpm200;

    void parseData(quint32 bytesRead);
    bool m_debug;

    quint8 STX { 0x02 };
    quint8 ETX { 0x03 };
    quint8 reportNumber { 0x00 };
};


class BpTruMessageHandler: public QObject
{
    Q_OBJECT
public slots:
    void process() {};

signals:
    void messageReceived(BPMMessage message);
};

#endif // BPTRU200DRIVER_H
