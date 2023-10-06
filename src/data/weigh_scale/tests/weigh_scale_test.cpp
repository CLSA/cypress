#include "weigh_scale_test.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>


WeighScaleTest::WeighScaleTest()
{
    setExpectedMeasurementCount(2);
}

bool WeighScaleTest::isValid() const
{
    //TODO: handle partial cases wherein we want to update the UI
    // with tests containing the minimum measurement count but
    // not the full expected count required for writing
    //
    bool okTest = (getMeasurementCount() == getExpectedMeasurementCount()) ||
                  (0 < getMeasurementCount());
    if(okTest)
    {
      foreach(const auto m, m_measurementList)
      {
        if(!m.isValid())
        {
          okTest = false;
          break;
        }
      }
    }
    return okTest;
}

QString WeighScaleTest::toString() const
{
    QString str;
    if(isValid())
    {
      QStringList list;
      foreach(const auto m, m_measurementList)
      {
        list << m.toString();
      }
      str = list.join("\n");
    }
    return str;
}

void WeighScaleTest::simulate()
{
    WeightMeasurement weight1;
    weight1.simulate();

    WeightMeasurement weight2;
    weight2.simulate();

    addMeasurement(weight1);
    addMeasurement(weight2);
}

void WeighScaleTest::fromArray(const QByteArray &arr)
{
    if(!arr.isEmpty())
    {
       // only add to the end and keep the last two tests
       //
       WeightMeasurement m;
       m.fromArray(arr);

       if(m.isValid())
       {
         bool ok = true;
         if(0 < getMeasurementCount())
         {
           Measurement last = lastMeasurement();
           QDateTime prev = last.getAttributeValue("TIMESTAMP").toDateTime();
           QDateTime curr = m.getAttributeValue("TIMESTAMP").toDateTime();
           ok =  DELAY < prev.secsTo(curr);
         }
         if(ok)
           addMeasurement(m);
       }
    }
}

QJsonObject WeighScaleTest::toJsonObject() const
{
    QJsonObject testJson {
    };

    QJsonArray measurementArray;
    auto measurements { getMeasurements() };

    foreach(auto measurement, measurements)
    {
        measurementArray << measurement.toJsonObject();
    }

    QJsonObject valuesObject {};

    valuesObject.insert("results", measurementArray);


    double weight1 = valuesObject["results"].toArray()[0].toObject().value("weight").toObject().value("value").toDouble();
    double weight2 = valuesObject["results"].toArray()[1].toObject().value("weight").toObject().value("value").toDouble();
    double avg_weight { (weight1 + weight2) / 2 };

    valuesObject.insert("manual_entry", getManualEntryMode());
    valuesObject.insert("average_weight", avg_weight);

    testJson.insert("value", valuesObject);

    return testJson;
}
