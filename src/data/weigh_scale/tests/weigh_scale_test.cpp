#include "weigh_scale_test.h"
#include "data/weigh_scale/measurements/weight_measurement.h"
#include "auxiliary/Utilities.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

const double LBS_TO_KG = 0.453592;

double WeighScaleTest::convertWeightToKg(const QSharedPointer<Measurement> measure) {
    const double weight = measure->getAttributeValue("weight").toDouble();
    const QString unit = measure->getAttribute("weight").units();
    return unit == "kg" ? weight : weight * LBS_TO_KG;
}

WeighScaleTest::WeighScaleTest()
{
    m_outputKeyList << "average_weight";
    setExpectedMeasurementCount(2);
}

bool WeighScaleTest::isValid() const
{
    //TODO: handle partial cases wherein we want to update the UI
    // with tests containing the minimum measurement count but
    // not the full expected count required for writing
    //
    bool okTest = getMeasurementCount() > 1;
    if (okTest) {
        foreach (const auto m, m_measurementList) {
            if (!m->isValid()) {
                okTest = false;
                break;
            }
        }
    }
    return okTest;
}

void WeighScaleTest::reinterpret()
{
    calculateAverage();
}

double WeighScaleTest::calculateAverage()
{
    //qDebug() << "WeighScaleTest::calculateAverage";
    //qDebug() << toString();
    //qDebug() << getMeasurements().length();

    auto &measurements = getMeasurements();
    double average = 0;
    double totalWeightKg = 0;

    if (measurements.length() < 1)
    {
        //reset();
        return -1;
    }

    int validMeasures = 0;
    for (auto &measure : measurements) {
        if (measure->isValid()) {
            totalWeightKg += convertWeightToKg(measure);
            validMeasures++;
        }
    }

    if (validMeasures == 0)
        return -1;

    average = totalWeightKg / validMeasures;
    addMetaData("average_weight", Utilities::round_to(average, 1), "kg");

    //qDebug() << "average" << average << "kg";

    return average;
}

QString WeighScaleTest::toString() const
{
    QString str;
    if(isValid())
    {
      QStringList list;
      foreach (const auto m, m_measurementList) {
          list << m->toString();
      }
      str = list.join("\n");
    }
    return str;
}

void WeighScaleTest::fromArray(const QByteArray &arr)
{
    if (arr.isEmpty())
        return;

    QSharedPointer<WeightMeasurement> m(new WeightMeasurement);
    m->fromArray(arr);

    if (m->isValid())
        addMeasurement(m);

    calculateAverage();
}

QJsonObject WeighScaleTest::toJsonObject() const
{
    QJsonObject testJson {};
    QJsonArray measurementArray {};

    foreach (auto measurement, m_measurementList) {
        if (measurement->isValid())
            measurementArray << measurement->toJsonObject();
    }

    QJsonObject metadata = getMetaData().toJsonObject();
    testJson.insert("results", measurementArray);
    testJson.insert("manual_entry", getManualEntryMode());
    testJson.insert("metadata", metadata);

    return testJson;
}


