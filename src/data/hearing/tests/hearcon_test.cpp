#include "hearcon_test.h"
#include "auxiliary/json_settings.h"

#include "data/hearing/measurements/hearcon_measurement.h"

HearconTest::HearconTest()
{

}

void HearconTest::fromJsonFile(const QString &filePath)
{
    QJsonObject json = JsonSettings::readJsonFromFile(filePath);

    addMetaData("test_id", 		json.value("test_id"));
    addMetaData("empl_num", 	json.value("empl_num"));
    addMetaData("comp_id", 		json.value("comp_id"));
    addMetaData("plan_id", 		json.value("plan_id"));
    addMetaData("entry_date", 	json.value("entry_date"));
    addMetaData("test_date", 	json.value("test_date"));
    addMetaData("test_type", 	json.value("test_type"));
    addMetaData("q1", 			json.value("q1"));
    addMetaData("q2", 			json.value("q2"));
    addMetaData("q3", 			json.value("q3"));
    addMetaData("q4", 			json.value("q4"));
    addMetaData("e_q5", 		json.value("e_q5"));
    addMetaData("e_q6", 		json.value("e_q6"));
    addMetaData("e_q7", 		json.value("e_q7"));
    addMetaData("e_q8", 		json.value("e_q8"));
    addMetaData("e_q9", 		json.value("e_q9"));
    addMetaData("e_q10", 		json.value("e_q10"));
    addMetaData("q6", 			json.value("q6"));
    addMetaData("q7", 			json.value("q7"));
    addMetaData("comments", 	json.value("comments"));
    addMetaData("a_make", 		json.value("a_make"));
    addMetaData("a_model", 		json.value("a_model"));
    addMetaData("a_serial", 	json.value("a_serial"));
    addMetaData("a_calibration",json.value("a_calibration"));
    addMetaData("sts_r", 		json.value("sts_r"));
    addMetaData("bit", 			json.value("bit"));
    addMetaData("sts_l", 		json.value("sts_l"));
    addMetaData("rec_r", 		json.value("rec_r"));
    addMetaData("rec_l", 		json.value("rec_l"));
    addMetaData("ews_r", 		json.value("ews_r"));
    addMetaData("ews_l", 		json.value("ews_l"));
    addMetaData("sts_tr", 		json.value("sts_tr"));
    addMetaData("sts_tl", 		json.value("sts_tl"));
    addMetaData("rec_tr", 		json.value("rec_tr"));
    addMetaData("sts_cr", 		json.value("sts_cr"));
    addMetaData("sts_cl", 		json.value("sts_cl"));
    addMetaData("rec_cl", 		json.value("rec_cl"));
    addMetaData("rec_cr", 		json.value("rec_cr"));
    addMetaData("ews_cr", 		json.value("ews_cr"));
    addMetaData("ews_cl", 		json.value("ews_cl"));
    addMetaData("age_used", 	json.value("age_used"));
    addMetaData("audio_in", 	json.value("audio_in"));
    addMetaData("eGUID", 		json.value("eGUID"));
    addMetaData("tGUID", 		json.value("tGUID"));
    addMetaData("examinerName", json.value("examinerName"));

    addMetaData("slm_used", 	json.value("slm_used"));
    addMetaData("slm125", 		json.value("slm125"));
    addMetaData("slm250", 		json.value("slm250"));
    addMetaData("slm500", 		json.value("slm500"));
    addMetaData("slm1000", 		json.value("slm1000"));
    addMetaData("slm2000", 		json.value("slm2000"));
    addMetaData("slm4000", 		json.value("slm4000"));
    addMetaData("slm8000", 		json.value("slm8000"));

    addMetaData("testDuration", json.value("testDuration"));
    addMetaData("pausedCount", 	json.value("pausedCount"));
    addMetaData("otoRight", 	json.value("otoRight"));
    addMetaData("otoLeft", 		json.value("otoLeft"));
    addMetaData("twa", 			json.value("twa"));
    addMetaData("isReviewed", 	json.value("isReviewed"));
    addMetaData("revBatch", 	json.value("revBatch"));

    QSharedPointer<HearconMeasurement> l250(new HearconMeasurement);
    l250->setAttribute("side",     "left");
    l250->setAttribute("test",     "l250");
    l250->setAttribute("level",    json.value("l250"), "dB");
    l250->setAttribute("pass",     json.value("l250").toInt() <= 40);
    l250->setAttribute("outcome",  "");
    l250->setAttribute("error",    "");
    addMeasurement(l250);

    QSharedPointer<HearconMeasurement> l500(new HearconMeasurement);
    l500->setAttribute("side",     "left");
    l500->setAttribute("test",     "l500");
    l500->setAttribute("level",    json.value("l500"), "dB");
    l500->setAttribute("pass",     json.value("l500").toInt() <= 40);
    l500->setAttribute("outcome",  "");
    l500->setAttribute("error",    "");
    addMeasurement(l500);

    QSharedPointer<HearconMeasurement> l1000(new HearconMeasurement);
    l1000->setAttribute("side",     "left");
    l1000->setAttribute("test",     "l1000");
    l1000->setAttribute("level",    json.value("l1000"), "dB");
    l1000->setAttribute("pass",     json.value("l1000").toInt() <= 40);
    l1000->setAttribute("outcome",  "");
    l1000->setAttribute("error",    "");
    addMeasurement(l1000);

    QSharedPointer<HearconMeasurement> l1500(new HearconMeasurement);
    l1500->setAttribute("side",     "left");
    l1500->setAttribute("test",     "l1500");
    l1500->setAttribute("level",    json.value("l1500"), "dB");
    l1500->setAttribute("pass",     json.value("l1500").toInt() <= 40);
    l1500->setAttribute("outcome",  "");
    l1500->setAttribute("error",    "");
    addMeasurement(l1500);

    QSharedPointer<HearconMeasurement> l2000(new HearconMeasurement);
    l2000->setAttribute("side",     "left");
    l2000->setAttribute("test",     "l2000");
    l2000->setAttribute("level",    json.value("l2000"), "dB");
    l2000->setAttribute("pass",     json.value("l2000").toInt() <= 40);
    l2000->setAttribute("outcome",  "");
    l2000->setAttribute("error",    "");
    addMeasurement(l2000);

    QSharedPointer<HearconMeasurement> l3000(new HearconMeasurement);
    l3000->setAttribute("side",     "left");
    l3000->setAttribute("test",     "l3000");
    l3000->setAttribute("level",    json.value("l3000"), "dB");
    l3000->setAttribute("pass",     json.value("l3000").toInt() <= 40);
    l3000->setAttribute("outcome",  "");
    l3000->setAttribute("error",    "");
    addMeasurement(l3000);

    QSharedPointer<HearconMeasurement> l4000(new HearconMeasurement);
    l4000->setAttribute("side",     "left");
    l4000->setAttribute("test",     "l4000");
    l4000->setAttribute("level",    json.value("l4000"), "dB");
    l4000->setAttribute("pass",     json.value("l4000").toInt() <= 40);
    l4000->setAttribute("outcome",  "");
    l4000->setAttribute("error",    "");
    addMeasurement(l4000);

    QSharedPointer<HearconMeasurement> l6000(new HearconMeasurement);
    l6000->setAttribute("side",     "left");
    l6000->setAttribute("test",     "l6000");
    l6000->setAttribute("level",    json.value("l6000"), "dB");
    l6000->setAttribute("pass",     json.value("l6000").toInt() <= 40);
    l6000->setAttribute("outcome",  "");
    l6000->setAttribute("error",    "");
    addMeasurement(l6000);

    QSharedPointer<HearconMeasurement> l8000(new HearconMeasurement);
    l8000->setAttribute("side",     "left");
    l8000->setAttribute("test",     "l8000");
    l8000->setAttribute("level",    json.value("l8000"), "dB");
    l8000->setAttribute("pass",     json.value("l8000").toInt() <= 40);
    l8000->setAttribute("outcome",  "");
    l8000->setAttribute("error",    "");
    addMeasurement(l8000);

    QSharedPointer<HearconMeasurement> r250(new HearconMeasurement);
    r250->setAttribute("side",     "right");
    r250->setAttribute("test",     "r250");
    r250->setAttribute("level",    json.value("r250"), "dB");
    r250->setAttribute("pass",     json.value("r250").toInt() <= 40);
    r250->setAttribute("outcome",  "");
    r250->setAttribute("error",    "");
    addMeasurement(r250);

    QSharedPointer<HearconMeasurement> r500(new HearconMeasurement);
    r500->setAttribute("side",     "right");
    r500->setAttribute("test",     "r500");
    r500->setAttribute("level",    json.value("r500"), "dB");
    r500->setAttribute("pass",     json.value("r500").toInt() <= 40);
    r500->setAttribute("outcome",  "");
    r500->setAttribute("error",    "");
    addMeasurement(r500);

    QSharedPointer<HearconMeasurement> r1000(new HearconMeasurement);
    r1000->setAttribute("side",     "right");
    r1000->setAttribute("test",     "r1000");
    r1000->setAttribute("level",    json.value("r1000"), "dB");
    r1000->setAttribute("pass",     json.value("r1000").toInt() <= 40);
    r1000->setAttribute("outcome",  "");
    r1000->setAttribute("error",    "");
    addMeasurement(r1000);

    QSharedPointer<HearconMeasurement> r1500(new HearconMeasurement);
    r1500->setAttribute("side",     "right");
    r1500->setAttribute("test",     "r1500");
    r1500->setAttribute("level",    json.value("r1500"), "dB");
    r1500->setAttribute("pass",     json.value("r1500").toInt() <= 40);
    r1500->setAttribute("outcome",  "");
    r1500->setAttribute("error",    "");
    addMeasurement(r1500);

    QSharedPointer<HearconMeasurement> r2000(new HearconMeasurement);
    r2000->setAttribute("side",     "right");
    r2000->setAttribute("test",     "r2000");
    r2000->setAttribute("level",    json.value("r2000"), "dB");
    r2000->setAttribute("pass",     json.value("r2000").toInt() <= 40);
    r2000->setAttribute("outcome",  "");
    r2000->setAttribute("error",    "");
    addMeasurement(r2000);

    QSharedPointer<HearconMeasurement> r3000(new HearconMeasurement);
    r3000->setAttribute("side",     "right");
    r3000->setAttribute("test",     "r3000");
    r3000->setAttribute("level",    json.value("r3000"), "dB");
    r3000->setAttribute("pass",     json.value("r3000").toInt() <= 40);
    r3000->setAttribute("outcome",  "");
    r3000->setAttribute("error",    "");
    addMeasurement(r3000);

    QSharedPointer<HearconMeasurement> r4000(new HearconMeasurement);
    r4000->setAttribute("side",     "right");
    r4000->setAttribute("test",     "r4000");
    r4000->setAttribute("level",    json.value("r4000"), "dB");
    r4000->setAttribute("pass",     json.value("r4000").toInt() <= 40);
    r4000->setAttribute("outcome",  "");
    r4000->setAttribute("error",    "");
    addMeasurement(r4000);

    QSharedPointer<HearconMeasurement> r6000(new HearconMeasurement);
    r6000->setAttribute("side",     "right");
    r6000->setAttribute("test",     "r6000");
    r6000->setAttribute("level",    json.value("r6000"), "dB");
    r6000->setAttribute("pass",     json.value("r6000").toInt() <= 40);
    r6000->setAttribute("outcome",  "");
    r6000->setAttribute("error",    "");
    addMeasurement(r6000);

    QSharedPointer<HearconMeasurement> r8000(new HearconMeasurement);
    r8000->setAttribute("side",     "right");
    r8000->setAttribute("test",     "r8000");
    r8000->setAttribute("level",    json.value("r8000"), "dB");
    r8000->setAttribute("pass",     json.value("r8000").toInt() <= 40);
    r8000->setAttribute("outcome",  "");
    r8000->setAttribute("error",    "");
    addMeasurement(r8000);
}

QString HearconTest::toString() const
{
    return "HearconTest";
}

bool HearconTest::isValid() const
{
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
        if (side == "left")
            leftResults.append(measureJson);
        else if (side == "right")
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

}

void HearconTest::reinterpret()
{

}
