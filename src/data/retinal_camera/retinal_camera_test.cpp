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
