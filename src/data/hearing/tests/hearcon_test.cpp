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

    QSharedPointer<HearconMeasurement> l500(new HearconMeasurement);
    l500->setAttribute("side",     "Left");
    l500->setAttribute("test",     "500 Hz");
    l500->setAttribute("level",    json.value("l500"), "dB");
    l500->setAttribute("pass",     json.value("l500").toInt() <= 40);
    l500->setAttribute("outcome",  "");
    l500->setAttribute("error",    "");
    addMeasurement(l500);

    QSharedPointer<HearconMeasurement> l1000(new HearconMeasurement);
    l1000->setAttribute("side",     "Left");
    l1000->setAttribute("test",     "1000 Hz");
    l1000->setAttribute("level",    json.value("l1000"), "dB");
    l1000->setAttribute("pass",     json.value("l1000").toInt() <= 40);
    l1000->setAttribute("outcome",  "");
    l1000->setAttribute("error",    "");
    addMeasurement(l1000);

    QSharedPointer<HearconMeasurement> l2000(new HearconMeasurement);
    l2000->setAttribute("side",     "Left");
    l2000->setAttribute("test",     "2000 Hz");
    l2000->setAttribute("level",    json.value("l2000"), "dB");
    l2000->setAttribute("pass",     json.value("l2000").toInt() <= 40);
    l2000->setAttribute("outcome",  "");
    l2000->setAttribute("error",    "");
    addMeasurement(l2000);

    QSharedPointer<HearconMeasurement> l3000(new HearconMeasurement);
    l3000->setAttribute("side",     "Left");
    l3000->setAttribute("test",     "3000 Hz");
    l3000->setAttribute("level",    json.value("l3000"), "dB");
    l3000->setAttribute("pass",     json.value("l3000").toInt() <= 40);
    l3000->setAttribute("outcome",  "");
    l3000->setAttribute("error",    "");
    addMeasurement(l3000);

    QSharedPointer<HearconMeasurement> l4000(new HearconMeasurement);
    l4000->setAttribute("side",     "Left");
    l4000->setAttribute("test",     "4000 Hz");
    l4000->setAttribute("level",    json.value("l4000"), "dB");
    l4000->setAttribute("pass",     json.value("l4000").toInt() <= 40);
    l4000->setAttribute("outcome",  "");
    l4000->setAttribute("error",    "");
    addMeasurement(l4000);

    QSharedPointer<HearconMeasurement> l6000(new HearconMeasurement);
    l6000->setAttribute("side",     "Left");
    l6000->setAttribute("test",     "6000 Hz");
    l6000->setAttribute("level",    json.value("l6000"), "dB");
    l6000->setAttribute("pass",     json.value("l6000").toInt() <= 40);
    l6000->setAttribute("outcome",  "");
    l6000->setAttribute("error",    "");
    addMeasurement(l6000);

    QSharedPointer<HearconMeasurement> l8000(new HearconMeasurement);
    l8000->setAttribute("side",     "Left");
    l8000->setAttribute("test",     "8000 Hz");
    l8000->setAttribute("level",    json.value("l8000"), "dB");
    l8000->setAttribute("pass",     json.value("l8000").toInt() <= 40);
    l8000->setAttribute("outcome",  "");
    l8000->setAttribute("error",    "");
    addMeasurement(l8000);

    QSharedPointer<HearconMeasurement> r500(new HearconMeasurement);
    r500->setAttribute("side",     "Right");
    r500->setAttribute("test",     "500 Hz");
    r500->setAttribute("level",    json.value("r500"), "dB");
    r500->setAttribute("pass",     json.value("r500").toInt() <= 40);
    r500->setAttribute("outcome",  "");
    r500->setAttribute("error",    "");
    addMeasurement(r500);

    QSharedPointer<HearconMeasurement> r1000(new HearconMeasurement);
    r1000->setAttribute("side",     "Right");
    r1000->setAttribute("test",     "1000 Hz");
    r1000->setAttribute("level",    json.value("r1000"), "dB");
    r1000->setAttribute("pass",     json.value("r1000").toInt() <= 40);
    r1000->setAttribute("outcome",  "");
    r1000->setAttribute("error",    "");
    addMeasurement(r1000);


    QSharedPointer<HearconMeasurement> r2000(new HearconMeasurement);
    r2000->setAttribute("side",     "Right");
    r2000->setAttribute("test",     "2000 Hz");
    r2000->setAttribute("level",    json.value("r2000"), "dB");
    r2000->setAttribute("pass",     json.value("r2000").toInt() <= 40);
    r2000->setAttribute("outcome",  "");
    r2000->setAttribute("error",    "");
    addMeasurement(r2000);

    QSharedPointer<HearconMeasurement> r3000(new HearconMeasurement);
    r3000->setAttribute("side",     "Right");
    r3000->setAttribute("test",     "3000 Hz");
    r3000->setAttribute("level",    json.value("r3000"), "dB");
    r3000->setAttribute("pass",     json.value("r3000").toInt() <= 40);
    r3000->setAttribute("outcome",  "");
    r3000->setAttribute("error",    "");
    addMeasurement(r3000);

    QSharedPointer<HearconMeasurement> r4000(new HearconMeasurement);
    r4000->setAttribute("side",     "Right");
    r4000->setAttribute("test",     "4000 Hz");
    r4000->setAttribute("level",    json.value("r4000"), "dB");
    r4000->setAttribute("pass",     json.value("r4000").toInt() <= 40);
    r4000->setAttribute("outcome",  "");
    r4000->setAttribute("error",    "");
    addMeasurement(r4000);

    QSharedPointer<HearconMeasurement> r6000(new HearconMeasurement);
    r6000->setAttribute("side",     "Right");
    r6000->setAttribute("test",     "6000 Hz");
    r6000->setAttribute("level",    json.value("r6000"), "dB");
    r6000->setAttribute("pass",     json.value("r6000").toInt() <= 40);
    r6000->setAttribute("outcome",  "");
    r6000->setAttribute("error",    "");
    addMeasurement(r6000);

    QSharedPointer<HearconMeasurement> r8000(new HearconMeasurement);
    r8000->setAttribute("side",     "Right");
    r8000->setAttribute("test",     "8000 Hz");
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
