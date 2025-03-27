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
private:
    QJsonValue dfs(const QDomNode& node);

    bool hasMultipleChildrenOfSameType(const QDomNode& parentNode, const QString& tagName);

    QSharedPointer<Mac5Session> m_session;
};

#endif // MAC5_TEST_H
