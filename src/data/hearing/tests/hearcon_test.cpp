#include "hearcon_test.h"
#include "auxiliary/json_settings.h"

#include "data/hearing/measurements/hearcon_measurement.h"

HearconTest::HearconTest() {}

void HearconTest::fromJsonFile(const QString &filePath)
{
    QJsonObject json = JsonSettings::readJsonFromFile(filePath);
    QSharedPointer<HearconMeasurement> measure(new HearconMeasurement);
    for (auto it = json.begin(); it != json.end(); ++it) {
        const QString& key = it.key();
        const QVariant& value = it.value().toVariant();
        measure->setAttribute(key, value);
    }

    addMeasurement(measure);
}

QString HearconTest::toString() const
{
    return "HearconTest";
}

bool HearconTest::isValid() const
{
    return false;
}

QJsonObject HearconTest::toJsonObject() const
{
    QJsonObject json {};

    QJsonArray measurements;
    foreach (const auto m, m_measurementList) {
        QJsonObject measureJson = m->toJsonObject();
        measurements.append(measureJson);
    }

    json.insert("metadata", m_metaData.toJsonObject());
    json.insert("results", measurements);
    json.insert("manual_entry", getManualEntryMode());

    return json;
}

void HearconTest::reset()
{

}

void HearconTest::reinterpret()
{

}
