#ifndef MAC5_TEST_H
#define MAC5_TEST_H

#include "data/test_base.h"

#include <QJsonObject>
#include <QDomElement>

class Mac5Test : public TestBase
{
public:
    Mac5Test();

    // TestBase interface
public:
    QString toString() const;
    bool isValid() const;
    QJsonObject toJsonObject() const;

    QJsonDocument fromXmlFile(const QString& filePath);
private:
    QJsonObject dfs(QDomNode node);
};

#endif // MAC5_TEST_H
