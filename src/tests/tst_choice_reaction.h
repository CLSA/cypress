#ifndef TST_CHOICE_REACTION_H
#define TST_CHOICE_REACTION_H

#include <QTest>
#include <QJsonObject>

class TestChoiceReaction: public QObject
{
    Q_OBJECT

public:
    TestChoiceReaction();
    ~TestChoiceReaction();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};

#endif // TST_CHOICE_REACTION_H
