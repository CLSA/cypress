#ifndef CIMTVIVIDITEST_H
#define CIMTVIVIDITEST_H

#include "test_base.h"

class CimtVividiTest : public TestBase
{
public:
    CimtVividiTest();

    bool isValid() const override;

    void reset() override;

    QJsonObject toJsonObject() const override;
    QString toString() const override;

private:
    QStringList m_outputKeyList;
    QMap<QString, QString> m_displayNames;
};

#endif // CIMTVIVIDITEST_H
