#ifndef WHOLE_BODY_TEST_H
#define WHOLE_BODY_TEST_H

#include <QString>
#include <QList>

#include "data/test_base.h"

class WholeBodyScanTest : public TestBase
{
public:
    explicit WholeBodyScanTest();

    bool isValid() const override;
    void reset() override;

    QString toString() const override;
    QJsonObject toJsonObject() const override;

private:
    static const QList<QString> m_inputKeyList;
    static const QList<QString> m_outputKeyList;
};

#endif // WHOLE_BODY_TEST_H
