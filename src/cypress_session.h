#ifndef CYPRESSSESSION_H
#define CYPRESSSESSION_H

#include "dialogs/dialog_base.h"

#include <QObject>
#include <QJsonObject>
#include <QException>

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


class NotInstalledError: public QException {
public:
    NotInstalledError(std::string message) : message(message) {}

    void raise() const override {
        throw *this;
    }

    NotInstalledError* clone() const override {
        return new NotInstalledError(*this);
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
private:
    std::string message;

};

class NotAvailableError: public QException {
public:
    NotAvailableError(std::string message) : message(message) {}

    void raise() const override {
        throw *this;
    }

    NotAvailableError* clone() const override {
        return new NotAvailableError(*this);
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
private:
    std::string message;

};


class CypressSession : public QObject
{
    Q_OBJECT
public:
    enum SessionStatus {
        Unknown,
        Started,
        CriticalError,
        Success,
        Cancelled,
    };
    CypressSession(QObject* parent, const QJsonObject& inputData, const QString& origin);

    // Delete copy constructor and copy assignment operator.
    CypressSession(const CypressSession&) = delete;
    CypressSession& operator=(const CypressSession&) = delete;

    void setOrigin(const QString& originHeader);
    virtual ~CypressSession() {};

    virtual void initializeDialog() = 0;

    virtual void isInstalled() const { throw NotInstalledError("device is not installed"); };
    virtual void isAvailable() const { throw NotAvailableError("device is not available"); };

    // Determines if the basic key/value pairs exist for a session or throws exception
    virtual void validate() const;

    // Calculates any needed implicit variables from data sent from pine
    virtual void calculateInputs() {};

    virtual void start();
    virtual void end(const CypressSession::SessionStatus& status = CypressSession::SessionStatus::Success);

    virtual QString getWebpageContents() const
    {
        return
            "<!DOCTYPE html>"
            "<html lang=\"en\">"
            "<head>"
            "<meta charset=\"UTF-8\">"
            "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
            "<title>Pine</title>"
            "</head>"
            "<body>"
            "</body>"
            "</html>";
    };

    SessionStatus getStatus() const;
    QJsonObject getInputData() const;

    int getAnswerId() const;
    QString getUID() const;
    QString getSessionId() const;
    QString getBarcode() const;
    QString getLanguage() const;
    QString getInterviewer() const;
    QDateTime getStartTime() const;
    QDateTime getEndTime() const;
    QString getOrigin() const;

    QJsonObject getJsonObject() const;

protected:
    SessionStatus m_status { SessionStatus::Unknown };

    QString m_origin;

    // the device UI
    //
    DialogBase* m_dialog;

    // request body sent from Pine
    //
    QJsonObject m_inputData;

    QString m_interviewer {};
    QString m_uid {};
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



#endif // CYPRESSSESSION_H
