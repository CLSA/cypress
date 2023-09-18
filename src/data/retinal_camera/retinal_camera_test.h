#ifndef RETINALCAMERATEST_H
#define RETINALCAMERATEST_H

#include "data/TestBase.h"
#include "data/retinal_camera/retinal_camera_measurement.h"

class RetinalCameraTest : public TestBase<RetinalCameraMeasurement>
{
public:
    RetinalCameraTest();
};

#endif // RETINALCAMERATEST_H
