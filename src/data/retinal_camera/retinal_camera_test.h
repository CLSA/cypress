#ifndef RETINALCAMERATEST_H
#define RETINALCAMERATEST_H

#include "../test_base.h"

class RetinalCameraTest : public TestBase
{
public:
    RetinalCameraTest();

    void simulate(const QVariantMap& inputData) override;
    bool isValid() const override;

    QJsonObject toJsonObject() const override;
    QString toString() const override;

    bool fromDatabaseResults(const QJsonObject &results);

private:
    QStringList m_outputKeyList;
    QMap<QString, QString> m_displayNames;
};

#endif // RETINALCAMERATEST_H
