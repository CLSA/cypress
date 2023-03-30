#include <QtTest>
#include "tst_grip_strength.h"

#include "../managers/grip_strength/GripStrengthManager.h"

TestGripStrength::TestGripStrength()
{
}

void TestGripStrength::testInitialize()
{
    GripStrengthManager manager;
    manager.setTrackerDatabaseName("Tracker 5 Data");

    //QVERIFY2(manager.initialize(), "Failed to initialize database");

    // Ensure the database is open
    //QVERIFY2(manager.database.isOpen(), "Database is not open");
}

void TestGripStrength::testExtractExam()
{
    GripStrengthManager manager;
    manager.setTrackerDatabaseName("Tracker 5 Data");
    manager.initialize();

    QByteArray exam = manager.extractTrials();
    QFile file("grip_strength_test_output.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(exam);
        file.close();
    }

    // Ensure the exam fields are valid
    QVERIFY2(exam.contains("Rung"), "Exam does not contain Rung field");
    QVERIFY2(exam.contains("MaxReps"), "Exam does not contain MaxReps field");
    QVERIFY2(exam.contains("Sequence"), "Exam does not contain Sequence field");
    QVERIFY2(exam.contains("RestTime"), "Exam does not contain RestTime field");
    QVERIFY2(exam.contains("Rate"), "Exam does not contain Rate field");
    QVERIFY2(exam.contains("Threshold"), "Exam does not contain Threshold field");
    QVERIFY2(exam.contains("NormType"), "Exam does not contain NormType field");
    QVERIFY2(exam.contains("Comparison"), "Exam does not contain Comparison field");
    QVERIFY2(exam.contains("trials"), "Exam does not contain valid trials");
}
