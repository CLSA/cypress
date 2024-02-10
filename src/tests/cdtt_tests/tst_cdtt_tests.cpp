#include <QtTest>

// add necessary includes here

class cdtt_tests : public QObject
{
    Q_OBJECT

public:
    cdtt_tests();
    ~cdtt_tests();

private slots:
    void can_connect_to_spreadsheet();
};

cdtt_tests::cdtt_tests() {

}

cdtt_tests::~cdtt_tests() {

}

void cdtt_tests::can_connect_to_spreadsheet() {

}

QTEST_APPLESS_MAIN(cdtt_tests)

#include "tst_cdtt_tests.moc"
