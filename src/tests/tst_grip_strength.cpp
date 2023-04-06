#include <QtTest>
#include "tst_grip_strength.h"

#include "../managers/grip_strength/GripStrengthManager.h"

TestGripStrength::TestGripStrength()
{
}

void TestGripStrength::testInitialize()
{
}

void TestGripStrength::testExtractExam()
{
    GripStrengthManager manager;
    manager.start();
    manager.measure();

    // Ensure the exam fields are valid
    QVERIFY2(manager.m_exam.toJson().contains("Rung"), "Exam does not contain Rung field");
    QVERIFY2(manager.m_exam.toJson().contains("MaxReps"), "Exam does not contain MaxReps field");
    QVERIFY2(manager.m_exam.toJson().contains("Sequence"), "Exam does not contain Sequence field");
    QVERIFY2(manager.m_exam.toJson().contains("RestTime"), "Exam does not contain RestTime field");
    QVERIFY2(manager.m_exam.toJson().contains("Rate"), "Exam does not contain Rate field");
    QVERIFY2(manager.m_exam.toJson().contains("Threshold"), "Exam does not contain Threshold field");
    QVERIFY2(manager.m_exam.toJson().contains("NormType"), "Exam does not contain NormType field");
    QVERIFY2(manager.m_exam.toJson().contains("Comparison"), "Exam does not contain Comparison field");
    QVERIFY2(manager.m_exam.toJson().contains("trials"), "Exam does not contain valid trials");
}
