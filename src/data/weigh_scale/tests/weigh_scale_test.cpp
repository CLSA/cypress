#include "weigh_scale_test.h"
#include "data/weigh_scale/measurements/weight_measurement.h"
#include "auxiliary/Utilities.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

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
    bool okTest = getMeasurementCount() > 0;
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

double WeighScaleTest::calculateAverage()
{
    auto &measurements = getMeasurements();
    double average = 0;
    double totalWeight = 0;
    int numberOfMeasures = measurements.length();

    if (numberOfMeasures <= 0) {
        return 0.00;
    }

    for (auto &measure : measurements) {
        if (measure->hasAttribute("weight") && measure->getAttributeValue("weight").isValid()) {
            totalWeight += measure->getAttributeValue("weight").toDouble();
        }
    }

    average = totalWeight / numberOfMeasures;

    addMetaData("average_weight", Utilities::round_to(average, 0.1), "kg");

    qDebug() << "WeighScaleTest: new average" << average << "kg";

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

void WeighScaleTest::simulate()
{
    QSharedPointer<WeightMeasurement> weight1(new WeightMeasurement);
    weight1->simulate();

    QSharedPointer<WeightMeasurement> weight2(new WeightMeasurement);
    weight2->simulate();

    addMeasurement(weight1);
    addMeasurement(weight2);

    double average = calculateAverage();
    qDebug() << "WeighScaleTest::simulate - new average" << average << "kg";

    addMetaData("average_weight", Utilities::round_to(average, 0.1), "kg");
}

void WeighScaleTest::fromArray(const QByteArray &arr)
{
    if (arr.isEmpty()) {
        return;
    }

    QSharedPointer<WeightMeasurement> m(new WeightMeasurement);
    m->fromArray(arr);

    if (m->isValid()) {
        addMeasurement(m);
    }

    calculateAverage();
}

QJsonObject WeighScaleTest::toJsonObject() const
{
    QJsonObject testJson {};
    QJsonArray measurementArray {};

    foreach (auto measurement, m_measurementList) {
      measurementArray << measurement->toJsonObject();
    }

    QJsonObject metadata = getMetaData().toJsonObject();

    testJson.insert("results", measurementArray);
    testJson.insert("manual_entry", getManualEntryMode());
    testJson.insert("metadata", metadata);

    return testJson;
}
