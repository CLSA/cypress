#ifndef VALIDATORS_H
#define VALIDATORS_H

#include <QString>
#include <QJsonObject>
#include <QException>



namespace Validators {
    bool isValidString(const QJsonObject& input, const QString& key);
    bool isValidDouble(const QJsonObject& input, const QString& key);
    bool isValidInteger(const QJsonObject& input, const QString& key);
    bool isValidBool(const QJsonObject& input, const QString& key);
    bool isValidDate(const QJsonObject& input, const QString &key, const QString &dateFormat);
    bool isValidDateTime(const QJsonObject& input, const QString &key, const QString &dateFormat);

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
}

#endif // VALIDATORS_H
