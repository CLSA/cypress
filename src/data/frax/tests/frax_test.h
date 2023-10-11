#ifndef FRAX_TEST_H
#define FRAX_TEST_H

#include "../../test_base.h"

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

    void simulate(const QVariantMap&);

private:
    QStringList m_outputKeyList;

};

Q_DECLARE_METATYPE(FraxTest);

#endif // FRAX_TEST_H
