#include "hearcon_test.h"
#include "auxiliary/json_settings.h"

#include "data/hearing/measurements/hearcon_measurement.h"



QStringList metadataKeys {
    "test_id",
    "empl_num",
    "comp_id",
    "plan_id",
    "entry_date",
    "test_date",
    "test_type",
    "q1",
    "q2",
    "q3",
    "q4",
    "e_q5",
    "e_q6",
    "e_q7",
    "e_q8",
    "e_q9",
    "e_q10",
    "q6",
    "q7",
    "comments",
    "a_make",
    "a_model",
    "a_serial",
    "a_calibration",
    "sts_r",
    "bit",
    "sts_l",
    "rec_r",
    "rec_l",
    "ews_r",
    "ews_l",
    "sts_tr",
    "sts_tl",
    "rec_tr",
    "sts_cr",
    "sts_cl",
    "rec_cl",
    "rec_cr",
    "ews_cr",
    "ews_cl",
    "age_used",
    "audio_in",
    "eGUID",
    "tGUID",
    "examinerName",

    "slm_used",
    "slm125",
    "slm250",
    "slm500",
    "slm1000",
    "slm2000",
    "slm4000",
    "slm8000",

    "testDuration",
    "pausedCount",
    "otoRight",
    "otoLeft",
    "twa",
    "isReviewed",
    "revBatch",
};

QJsonObject measurementKeys {
    { "l500",  QJsonObject {
        { "side", "Left" },
        { "test", "500 Hz" }
    }},
    { "l1000", QJsonObject {
        { "side", "Left" },
        { "test", "1000 Hz" }
    }},
    { "l2000", QJsonObject {
        { "side", "Left" },
        { "test", "2000 Hz" }
    }},
    { "l3000", QJsonObject {
        { "side", "Left" },
        { "test", "3000 Hz" }
    }},
    { "l4000", QJsonObject {
        { "side", "Left" },
        { "test", "4000 Hz" }
    }},
    { "l6000", QJsonObject {
        { "side", "Left" },
        { "test", "6000 Hz" }
    }},
    { "l8000", QJsonObject {
        { "side", "Left" },
        { "test", "8000 Hz" }
    }},
    { "r500",  QJsonObject {
        { "side", "Right" },
        { "test", "500 Hz" }
    }},
    { "r1000", QJsonObject {
        { "side", "Right" },
        { "test", "1000 Hz" } }
    },
    { "r2000", QJsonObject {
        { "side", "Right" },
        { "test", "2000 Hz" }
    }},
    { "r3000", QJsonObject {
        { "side", "Right" },
        { "test", "3000 Hz" }
    }},
    { "r4000", QJsonObject {
        { "side", "Right" },
        { "test", "4000 Hz" }
    }},
    { "r6000", QJsonObject {
        { "side", "Right" },
        { "test", "6000 Hz" }
    }},
    { "r8000", QJsonObject {
        { "side", "Right" },
        { "test", "8000 Hz" }
    }}
};


HearconTest::HearconTest()
{

}


bool HearconTest::fromJsonFile(const QString &filePath)
{
    QJsonObject resultsJson = JsonSettings::readJsonFromFile(filePath);
    qDebug() << "Plugin Output: " << JsonSettings::prettyPrintJson(resultsJson);

    foreach (auto key, metadataKeys) {
        addMetaData(key, resultsJson.value(key));
    }

    foreach (auto key, measurementKeys.keys()) {
        auto value = resultsJson.value(key);

        qDebug() << value;

        if (value.isNull() || value.isUndefined()) {
            qDebug() << "HearconTest::fromJsonFile - " << key << "is invalid";
            return false;
        }

        QSharedPointer<HearconMeasurement> measure(new HearconMeasurement);
        measure->setAttribute("side",  measurementKeys.value(key).toObject().value("side").toString());
        measure->setAttribute("test",  measurementKeys.value(key).toObject().value("test").toString());
        measure->setAttribute("level", value.toDouble(), "db");
        measure->setAttribute("pass",  value.toDouble() <= 40);
        measure->setAttribute("outcome", "");
        measure->setAttribute("error",   "");

        if (!measure->isValid()) {
            qCritical() << "Measure is not valid";
            return false;
        }

        addMeasurement(measure);
    }

    return true;
}

QString HearconTest::toString() const
{
    return "HearconTest";
}

bool HearconTest::isValid() const
{
    if (m_measurementList.length() < 1)
        return false;

    for (auto it = m_measurementList.constBegin(); it != m_measurementList.constEnd(); ++it)
    {
        auto measure = *it;

        if (!measure->isValid())
            return false;
    }

    return true;
}

QJsonObject HearconTest::toJsonObject() const
{
    QJsonArray leftResults {};
    QJsonArray rightResults {};

    foreach (const auto m, m_measurementList) {
        QJsonObject measureJson = m->toJsonObject();
        QString side = measureJson["side"].toString();

        measureJson.remove("side");
        if (side == "Left")
            leftResults.append(measureJson);
        else if (side == "Right")
            rightResults.append(measureJson);
    }

    QJsonObject json {};
    json.insert("metadata", m_metaData.toJsonObject());
    json.insert("results", QJsonObject { { "left", leftResults }, { "right", rightResults }});
    json.insert("manual_entry", getManualEntryMode());

    return json;
}

void HearconTest::reset()
{
    TestBase::reset();
}

void HearconTest::reinterpret()
{

}
