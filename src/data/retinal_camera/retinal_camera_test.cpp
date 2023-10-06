#include "retinal_camera_test.h"
#include "retinal_camera_measurement.h"

#include <QJsonObject>
#include <QJsonArray>

RetinalCameraTest::RetinalCameraTest()
{
    m_outputKeyList << "EYE_PICT_VENDOR";
    m_outputKeyList << "EYE_SIDE_VENDOR";

    setExpectedMeasurementCount(2);
}

void RetinalCameraTest::simulate()
{
    RetinalCameraMeasurement leftEye;
    leftEye.setAttribute("EYE_PICT_VENDOR", "C:/Users/Anthony/Downloads/RETINAL_CAM_SIM/LEFT.jpg");
    leftEye.setAttribute("EYE_SIDE_VENDOR", "LEFT");

    RetinalCameraMeasurement rightEye;
    rightEye.setAttribute("EYE_PICT_VENDOR", "C:/Users/Anthony/Downloads/RETINAL_CAM_SIM/RIGHT.jpg");
    rightEye.setAttribute("EYE_SIDE_VENDOR", "RIGHT");

    this->addMeasurement(leftEye);
    this->addMeasurement(rightEye);
}

bool RetinalCameraTest::isValid() const
{
    if (getMeasurementCount() != getExpectedMeasurementCount()) {
        return false;
    }

    auto measurements = getMeasurements();
    foreach (auto measurement, measurements)
    {
        if (!measurement.isValid())
        {
            return false;
        }
    }

    return true;
}

QJsonObject RetinalCameraTest::toJsonObject() const
{
    QJsonObject testJson;
    QJsonArray measurementArray;

    auto measurements { getMeasurements() };

    foreach(auto measurement, measurements)
    {
        measurementArray << measurement.toJsonObject();
    }

    QJsonObject valuesObject {};

    valuesObject.insert("metadata", getMetaData().toJsonObject());
    valuesObject.insert("results", measurementArray);

    testJson.insert("values", valuesObject);

    return testJson;
}

QString RetinalCameraTest::toString() const
{
    return "";
}
