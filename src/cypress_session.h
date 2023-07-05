#ifndef CYPRESSSESSION_H
#define CYPRESSSESSION_H

#include "auxiliary/Constants.h"
#include "dialogs/dialog_base.h"

#include <QObject>
#include <QJsonObject>
#include <QException>

class CypressSession
{
public:
    CypressSession();
    CypressSession(const Constants::MeasureType& deviceType, const QJsonObject& inputData);
    CypressSession(const CypressSession& session);

    ~CypressSession();

    bool start();
    bool end();

    Constants::MeasureType getDeviceType() const;
    QJsonObject getInputData() const;

    QString getAnswerId() const;
    QString getSessionId() const;
    QString getBarcode() const;
    QString getLanguage() const;
    QString getInterviewer() const;

private:
    // The device to be used for this session
    const Constants::MeasureType m_deviceType;

    // The device UI
    QScopedPointer<DialogBase> m_deviceDialog;

    // Request body sent from Pine over the network
    const QJsonObject m_inputData;

    // Validated input data from m_inputData
    QString m_interviewer {};
    QString m_answerId {};
    QString m_sessionId {};
    QString m_barcode {};
    QString m_language {};


    QDateTime m_startDateTime {};
    QDateTime m_endDateTime {};

    // Determines if the basic key/value pairs exist for a session or throws exception
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
