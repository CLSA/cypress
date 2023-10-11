#ifndef HIP_TEST_H
#define HIP_TEST_H

#include "data/test_base.h"

class HipTest : public TestBase
{
public:
    explicit HipTest();

    void reset() override;
    bool isValid() const override;

    QString toString() const override;
    QJsonObject toJsonObject() const override;


private:
    const static QList<QString> m_outputKeyList;
    const static QList<QString> m_inputKeyList;
};

#endif // HIP_TEST_H
