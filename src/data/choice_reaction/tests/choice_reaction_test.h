#ifndef CHOICE_REACTION_TEST_H
#define CHOICE_REACTION_TEST_H

#include "data/test_base.h"

class ChoiceReactionTest : public TestBase
{
public:
    ChoiceReactionTest();
    ~ChoiceReactionTest() = default;

    void fromFile(const QString &);

    // String representation for debug and GUI display purposes
    //
    QString toString() const override;

    bool isValid() const override;

    // String keys are converted to snake_case
    //
    QJsonObject toJsonObject() const override;

    void simulate() override;

private:
    QList<QString> m_outputKeyList;

    bool m_debug;

};

Q_DECLARE_METATYPE(ChoiceReactionTest);

#endif // CHOICE_REACTION_TEST_H
