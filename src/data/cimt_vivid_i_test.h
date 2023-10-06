#ifndef CIMTVIVIDITEST_H
#define CIMTVIVIDITEST_H

#include "TestBase.h"

class CimtVividiTest : public TestBase<Measurement>
{
public:
    CimtVividiTest();

    void simulate();
    bool isValid() const override;

    QJsonObject toJsonObject() const override;
    QString toString() const override;

private:
    QStringList m_outputKeyList;
    QMap<QString, QString> m_displayNames;
};

#endif // CIMTVIVIDITEST_H
