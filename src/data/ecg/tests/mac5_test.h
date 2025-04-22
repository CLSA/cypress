#ifndef MAC5_TEST_H
#define MAC5_TEST_H

#include "data/test_base.h"
#include "server/sessions/mac5_session.h"

#include <QJsonObject>
#include <QDomElement>

class Mac5Test : public TestBase
{
public:
    Mac5Test(QSharedPointer<Mac5Session> session);

    // TestBase interface
public:
    QString toString() const;
    bool isValid() const;
    QJsonObject toJsonObject() const;

    QJsonDocument fromXmlFile(const QString& filePath);
    void validate() const;
private:
    QJsonValue dfs(const QDomNode& node);

    bool hasMultipleChildrenOfSameType(const QDomNode& parentNode, const QString& tagName);

    QSharedPointer<Mac5Session> m_session;
};

namespace Mac5 {
    class FilesMissingError: public QException {
        public:
            FilesMissingError(std::string message) : message(message) {}

            void raise() const override {
            throw *this;
            }

            FilesMissingError* clone() const override {
            return new FilesMissingError(*this);
            }

            const char* what() const noexcept override {
            return message.c_str();
            }

        private:
            std::string message;
    };

    class IncorrectBarcodeError: public QException {
        public:
            IncorrectBarcodeError(std::string message) : message(message) {}

            void raise() const override {
            throw *this;
            }

            IncorrectBarcodeError * clone() const override {
            return new IncorrectBarcodeError(*this);
            }

            const char* what() const noexcept override {
            return message.c_str();
            }

        private:
            std::string message;
    };
}

#endif // MAC5_TEST_H
