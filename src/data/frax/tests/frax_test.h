#ifndef FRAX_TEST_H
#define FRAX_TEST_H

#include "data/test_base.h"

class FraxTest : public TestBase
{
public:
    FraxTest();
    ~FraxTest() = default;

    void fromFile(const QString&);

    // String representation for debug and GUI display purposes
    //
    QString toString() const override;

    bool isValid() const override;

    // String keys are converted to snake_case
    //
    QJsonObject toJsonObject() const override;

private:
    QStringList m_outputKeyList;

    QString interpretResults(double p);

};

Q_DECLARE_METATYPE(FraxTest);

#endif // FRAX_TEST_H
