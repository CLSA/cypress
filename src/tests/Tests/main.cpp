#include <QtTest/QtTest>

#include "tst_pdf_filler.cpp"

// Include other test headers
int main(int argc, char *argv[]) {

    // Use QCoreApplication if no GUI
    QApplication app(argc, argv);

    int status = 0;

    {
        DeviceSimTests deviceSimTests;
        status |= QTest::qExec(&deviceSimTests, argc, argv);
    }
    {
        //MyTest2 test2;
        //status |= QTest::qExec(&test2, argc, argv);
    }
    // Run other tests

    return status; // Returns 0 if all tests pass
}
