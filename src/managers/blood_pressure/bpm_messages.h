#ifndef BPMMESSAGES_H
#define BPMMESSAGES_H

#include "bptru_200_driver.h"

class BpmError
{
public:
    BpmError(quint8 code)
    {
        m_code = code;
    }

    enum TestError {
        Indeterminate,
        UnderRange,
        OverRange,
        ArithmeticError,
        UnknownTestError,
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
        UnknownSystemError = 0xFF,
    };

    bool hasError()
    {
        return hasSystemError() || hasSbpError() || hasDbpError() || hasPulseError();
    }

    bool hasSystemError()
    {
        return (m_code & 0x80) == 0x80;
    }

    bool hasSbpError()
    {
        return (m_code & 0x40) == 0x40;
    }

    bool hasDbpError()
    {
        return (m_code & 0x20) == 0x20;
    }

    bool hasPulseError()
    {
        return (m_code & 0x10) == 0x10;
    }

    SystemError getSystemError()
    {
        switch(m_code)
        {
        case SystemError::TooFewPulses:
            return SystemError::TooFewPulses;
        case SystemError::TooManyMotionArtifacts:
            return SystemError::TooManyMotionArtifacts;
        case SystemError::Overpressure:
            return SystemError::Overpressure;
        case SystemError::InflationTooSlow:
            return SystemError::InflationTooSlow;
        case SystemError::InflationTooFast:
            return SystemError::InflationTooFast;
        case SystemError::DeflationTooSlow:
            return SystemError::DeflationTooSlow;
        case SystemError::DeflationTooFast:
            return SystemError::DeflationTooFast;
        case SystemError::SoftwareFault:
            return SystemError::SoftwareFault;
        case SystemError::PulseAmplitudeTooLow:
            return SystemError::PulseAmplitudeTooLow;
        case SystemError::PressureBelowMinus10:
            return SystemError::PressureBelowMinus10;
        case SystemError::PressureTooHighForTooLong:
            return SystemError::PressureTooHighForTooLong;
        case SystemError::PressureNotLowEnoughForTooLong:
            return SystemError::PressureNotLowEnoughForTooLong;
        case SystemError::STAM_LTAM_Violation:
            return SystemError::STAM_LTAM_Violation;
        case SystemError::ResetByMeansOtherThanPowerUp:
            return SystemError::ResetByMeansOtherThanPowerUp;
        case SystemError::ResetByMeansOtherThanPowerUpRecoveryDataCorrupted:
            return SystemError::ResetByMeansOtherThanPowerUpRecoveryDataCorrupted;
        case SystemError::BatteryTooLow:
            return SystemError::BatteryTooLow;
        default:
            qDebug() << "unknown system error code: " << m_code;
            return SystemError::UnknownSystemError;
        }
    }

    TestError getSystolicBloodPressureError()
    {
        switch(m_code)
        {
        case 0x14:
            return TestError::Indeterminate;
        case 0x17:
            return TestError::UnderRange;
        case 0x19:
            return TestError::OverRange;
        case 0x21:
            return TestError::ArithmeticError;
        default:
            qDebug() << "unknown systolic error code: " << m_code;
            return BpmError::TestError::UnknownTestError;
        }
    }

    TestError getDiastolicBloodPressureError()
    {
        switch (m_code)
        {
        case 0x15:
            return TestError::Indeterminate;
        case 0x20:
            return TestError::ArithmeticError;
        default:
            qDebug() << "unknown diastolic error code: " << m_code;
             return TestError::UnknownTestError;
        }
    }

    TestError getPulseError()
    {
        switch(m_code)
        {
        case 0x16:
            return TestError::Indeterminate;
        case 0x21:
            return TestError::ArithmeticError;
        default:
            qDebug() << "unknown pulse error code: " << m_code;
            return TestError::UnknownTestError;
        }
    }
private:
    quint8 m_code;
};



class AckMessage: public BPMMessage
{
    enum Type {
        AckHandshake = 0x00,
        AckStop = 0x01,
        AckReview = 0x02,
        AckCycle = 0x03,
        AckStart = 0x04,
        AckClear = 0x05,
    };
};


class ButtonMessage: public BPMMessage
{
};

class DataMessage: public BPMMessage
{
public:
    enum Type {
        BpInflCuffPressure = 0x49,
        BpDeflCuffPressure = 0x44,
        BpResult = 0x42,
        BpAvg = 0x41,
        BpReview = 0x52,
    };
};


class BloodPressureMessage: public DataMessage
{
public:
    int systolicBloodPressure()
    {
        return getData1() & 0x000000FF;
    }

    int diastolicBloodPressure()
    {
        return getData2() & 0x000000FF;
    }

    int pulse()
    {
        return getData3() & 0x000000FF;
    }

    BpmError code()
    {
        return BpmError(getData0());
    }

    bool hasError()
    {
        return code().hasError();
    }

    BpmError::TestError sbpError()
    {
        return code().getSystolicBloodPressureError();
    }

    BpmError::TestError dbpError()
    {
        return code().getDiastolicBloodPressureError();
    }

    BpmError::SystemError systemError()
    {
        return code().getSystemError();
    }
};

class CuffPressureMessage: public DataMessage
{
public:
    int pressure()
    {
        int ls = getData0() & 0x000000FF;
        int ms = (getData1() << 8) & 0x0000FF00;

        return ls + ms;
    }
};



class NotificationMessage: public BPMMessage
{

};


class ResetNotificationMessage: public BPMMessage
{

};

class AckHandshake: public AckMessage
{

};


class AckStop: public AckMessage
{

};

class AckReview: public AckMessage
{

};


class AckCycle: public AckMessage
{

};

class AckStart: public AckMessage
{

};

class AckClear: public AckMessage
{

};











#endif // BPMMESSAGES_H
