#include <QtTest>

#include "tst_retinal_camera.h"
#include "../managers/retinal_camera/RetinalCameraManager.h"


TestRetinalCamera::TestRetinalCamera()
{

}

void TestRetinalCamera::testInitialize()
{

}

void TestRetinalCamera::testExtractExam()
{
    RetinalCameraManager manager("");

    manager.start();
    //manager.measure();

    // Ensure the exam fields are valid
    QVERIFY2(manager.m_exam.toJson().contains("left"), "Exam does not contain left eye data");
    QVERIFY2(manager.m_exam.toJson().contains("right"), "Exam does not contain right eye data");
}
