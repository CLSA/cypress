#ifndef CYPRESSSESSION_H
#define CYPRESSSESSION_H

#include "dialogs/dialog_base.h"

#include <QObject>
#include <QJsonObject>
#include <QException>

enum SessionStatus {
    Unknown,
    Started,
    Ended,
};

class CypressSession : public QObject
{
    Q_OBJECT
public:
    CypressSession(QObject* parent, const QJsonObject& inputData);

    virtual ~CypressSession() {};

    // Determines if the basic key/value pairs exist for a session or throws exception
    virtual void validate() const;

    // Calculates any needed implicit variables from data sent from pine
    virtual void calculateInputs() {};

    virtual void start();
    virtual void end();

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

protected:
    SessionStatus m_status { SessionStatus::Unknown };

    // the device UI
    //
    QScopedPointer<DialogBase> m_dialog;

    // request body sent from Pine
    //
    QJsonObject m_inputData;

    QString m_interviewer {};
    int m_answerId {};
    QString m_sessionId {};
    QString m_barcode {};
    QString m_language {};

    QDateTime m_startDateTime {};
    QDateTime m_endDateTime {};

    bool isValidString(const QString& key) const;
    bool isValidDouble(const QString& key) const;
    bool isValidInteger(const QString& key) const;
    bool isValidBool(const QString& key) const;
    bool isValidDate(const QString &key, const QString &dateFormat) const;
    bool isValidDateTime(const QString &key, const QString &dateFormat) const;
};

class ValidationError: public QException {
public:
    ValidationError(std::string message) : message(message) {}

    void raise() const override {
        throw *this;
    }

    ValidationError* clone() const override {
        return new ValidationError(*this);
    }

    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};

#endif // CYPRESSSESSION_H
