#include "tst_choice_reaction.h"
#include "../data/choice_reaction/tests/ChoiceReactionTest.h"

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



