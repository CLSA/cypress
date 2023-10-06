#ifndef CYPRESSSESSION_H
#define CYPRESSSESSION_H

#include "auxiliary/Constants.h"
#include "dialogs/dialog_base.h"

#include <QObject>
#include <QJsonObject>
#include <QException>

enum SessionStatus {
    Unknown,
    Started,
    Ended,
};

class CypressSession
{
public:
    CypressSession();
    CypressSession(const Constants::MeasureType& deviceType, const QJsonObject& inputData);
    CypressSession(const Constants::ReportType& reportType, const QJsonObject& inputData);
    CypressSession(const CypressSession& session);

    ~CypressSession();

    void start();
    void end();

    void startReport();
    void endReport();

    Constants::MeasureType getDeviceType() const;
    Constants::ReportType getReportType() const;
    SessionStatus getStatus() const;

    QJsonObject getInputData() const;

    int getAnswerId() const;
    QString getSessionId() const;
    QString getBarcode() const;
    QString getLanguage() const;
    QString getInterviewer() const;

    QDateTime getStartTime() const;
    QDateTime getEndTime() const;

    QJsonObject getJsonObject() const;

private:
    // the device to be used for this session (if applicable)
    //
    const Constants::MeasureType m_deviceType { Constants::MeasureType::Unknown };

    // the report to be used for this session (if applicable)
    //
    const Constants::ReportType m_reportType { Constants::ReportType::reportTypeUnknown };


    SessionStatus m_status { SessionStatus::Unknown };

    // the device UI
    //
    QScopedPointer<DialogBase> m_deviceDialog;

    // request body sent from Pine over the network
    //
    QJsonObject m_inputData;

    // validated input data from m_inputData
    //
    QString m_interviewer {};
    int m_answerId {};
    QString m_sessionId {};
    QString m_barcode {};
    QString m_language {};

    QDateTime m_startDateTime {};
    QDateTime m_endDateTime {};

    // Determines if the basic key/value pairs exist for a session or throws exception
    //
    void validate(const QJsonObject& inputData) const;

    bool isValidString(const QJsonObject& inputData, const QString& key) const;
    bool isValidDouble(const QJsonObject& inputData, const QString& key) const;
    bool isValidInteger(const QJsonObject& inputData, const QString& key) const;
};

class InvalidBarcodeException: public QException
{
public:
    void raise() const {
        throw *this;
    }

    InvalidBarcodeException *clone() const {
        return new InvalidBarcodeException(*this);
    }

    const char* what() const noexcept {
        return "Barcode is invalid";
    }
};

class InvalidInterviewerException: public QException
{
public:
    void raise() const {
        throw *this;
    }

    InvalidInterviewerException* clone() const {
        return new InvalidInterviewerException(*this);
    }

    const char* what() const noexcept {
        return "interviewer";
    }
};

class InvalidAnswerIdException: public QException
{
public:
    void raise() const { throw *this; }

    InvalidAnswerIdException *clone() const {
        return new InvalidAnswerIdException(*this);
    }

    const char* what() const noexcept {
        return "answer_id";
    }
};


class InvalidLanguageException: public QException
{
public:
    void raise() const
    {
        throw *this;
    }

    InvalidLanguageException* clone() const
    {
        return new InvalidLanguageException(*this);
    }

    const char* what() const noexcept
    {
        return "language";
    }
};

class InvalidInputDataException: public QException
{
public:
    void raise() const
    {
        throw *this;
    }

    InvalidInputDataException* clone() const
    {
        return new InvalidInputDataException(*this);
    }

    const char* what() const noexcept
    {
        return "invalid input";
    }
};

#endif // CYPRESSSESSION_H
