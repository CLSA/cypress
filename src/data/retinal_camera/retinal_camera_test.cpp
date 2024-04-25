#include "retinal_camera_test.h"
#include "retinal_camera_measurement.h"

#include <QJsonObject>
#include <QJsonArray>

RetinalCameraTest::RetinalCameraTest()
{
    m_outputKeyList << "EYE_PICT_VENDOR";
    m_outputKeyList << "EYE_SIDE_VENDOR";

    setExpectedMeasurementCount(1);
}

void RetinalCameraTest::simulate(const QVariantMap& inputData)
{
    if (inputData["side"] == "left") {
        QSharedPointer<RetinalCameraMeasurement> leftEye(new RetinalCameraMeasurement);
        leftEye->setAttribute("EYE_PICT_VENDOR", "C:/Users/Anthony/Downloads/RETINAL_CAM_SIM/LEFT.jpg");
        leftEye->setAttribute("EYE_SIDE_VENDOR", "LEFT");
        this->addMeasurement(leftEye);
    }
    else if (inputData["side"] == "right") {
        QSharedPointer<RetinalCameraMeasurement> rightEye(new RetinalCameraMeasurement);
        rightEye->setAttribute("EYE_PICT_VENDOR",
                           "C:/Users/Anthony/Downloads/RETINAL_CAM_SIM/RIGHT.jpg");
        rightEye->setAttribute("EYE_SIDE_VENDOR", "RIGHT");
        this->addMeasurement(rightEye);
    }

}

bool RetinalCameraTest::fromDatabaseResults(const QJsonObject &results)
{
    qDebug() << "results: " << results;

    QSharedPointer<RetinalCameraMeasurement> eyeMeasure(new RetinalCameraMeasurement);
    if (results["side"] == "left") {
        eyeMeasure->setAttribute("EYE_PICT_VENDOR",
                                 results["filePath"].toString() + "/" + results["fileName"].toString() + ".jpg");
        eyeMeasure->setAttribute("EYE_SIDE_VENDOR", "LEFT");
    }

    else if (results["side"] == "right") {
        eyeMeasure->setAttribute("EYE_PICT_VENDOR",
                                 results["filePath"].toString() + "/" + results["fileName"].toString() + ".jpg");
        eyeMeasure->setAttribute("EYE_SIDE_VENDOR", "RIGHT");
    }

    addMeasurement(eyeMeasure);

    return true;
}

bool RetinalCameraTest::isValid() const
{
    if (getMeasurementCount() != getExpectedMeasurementCount()) {
        qDebug() << "measurement count" << getMeasurementCount() << "does not equal " << getExpectedMeasurementCount();
        return false;
    }

    const auto measurements = getMeasurements();
    foreach (auto measurement, measurements) {
        if (!measurement->isValid()) {
            qDebug() << "measurement is invalid:" << measurement->toJsonObject();
            return false;
        }
    }

    return true;
}

QJsonObject RetinalCameraTest::toJsonObject() const
{
    QJsonObject testJson;
    QJsonArray measurementArray;

    foreach (auto measurement, m_measurementList) {
        measurementArray << measurement->toJsonObject();
    }

    QJsonObject valuesObject {};

    valuesObject.insert("metadata", getMetaData().toJsonObject());
    valuesObject.insert("results", measurementArray);
    valuesObject.insert("manual_entry", getManualEntryMode());

    return valuesObject;
}

QString RetinalCameraTest::toString() const {
    return "";
}
