#include <QtTest>

// add necessary includes here

class ChoiceReactionTests : public QObject
{
    Q_OBJECT

public:
    ChoiceReactionTests();
    ~ChoiceReactionTests();

private slots:
    void test_case1();
};

ChoiceReactionTests::ChoiceReactionTests() {}

ChoiceReactionTests::~ChoiceReactionTests() {}

void ChoiceReactionTests::test_case1() {}

QTEST_APPLESS_MAIN(ChoiceReactionTests)

#include "tst_choicereactiontests.moc"
