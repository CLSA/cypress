#include <QtTest>

#include "../data/choice_reaction/tests/ChoiceReactionTest.h"

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

TestChoiceReaction::TestChoiceReaction()
{
    qDebug("constructor");
}

TestChoiceReaction::~TestChoiceReaction()
{

    qDebug("destructor");
}

void TestChoiceReaction::initTestCase()
{
}

void TestChoiceReaction::cleanupTestCase()
{
}

void TestChoiceReaction::testCase1()
{
    ChoiceReactionTest test;
    QVERIFY(test.isValid());
}


QTEST_APPLESS_MAIN(TestChoiceReaction)

#include "tst_choice_reaction.moc"
