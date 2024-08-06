#include "watch_bp_test.h"

#include <QJsonArray>

WatchBPTest::WatchBPTest()
{
    //m_outputKeyList << "Gender";
    //m_outputKeyList << "Age";
    //m_outputKeyList << "DOB";

    //m_outputKeyList << "SYS";
    //m_outputKeyList << "DIA";
    //m_outputKeyList << "DaySYS";
    //m_outputKeyList << "DayDIA";
    //m_outputKeyList << "NightSYS";
    //m_outputKeyList << "NightDIA";
}


bool WatchBPTest::isValid() const
{
    bool okTest = 1 <= getMeasurementCount();
    if (okTest) {
        foreach (const auto m, m_measurementList) {
            if (!m->isValid()) {
                okTest = false;
                break;
            }
        }
    }

    qDebug() << "valid: " << okTest;

    return okTest;
}

QJsonObject WatchBPTest::toJsonObject() const
{
    QJsonObject testJson {};
    QJsonArray measurementArray {};

    foreach (const auto m, m_measurementList) {
       measurementArray.append(m->toJsonObject());
    }

    QJsonObject valuesObject {};
    valuesObject.insert("results", measurementArray);
    valuesObject.insert("metadata", m_metaData.toJsonObject());
    valuesObject.insert("manual_entry", getManualEntryMode());

    return valuesObject;
}

void WatchBPTest::reset()
{

}

void WatchBPTest::reinterpret()
{
    updateAverage();
}

void WatchBPTest::updateAverage()
{
    const int n = getMeasurementCount();

    if (n <= 1) {
        removeMetaData("first_systolic");
        removeMetaData("first_diastolic");
        removeMetaData("first_pulse");
        removeMetaData("first_start_time");
        removeMetaData("first_end_time");

        removeMetaData("avg_count");
        removeMetaData("avg_systolic");
        removeMetaData("avg_diastolic");
        removeMetaData("avg_pulse");

        removeMetaData("total_avg_systolic");
        removeMetaData("total_avg_diastolic");
        removeMetaData("total_avg_pulse");
        removeMetaData("total_avg_count");

        return;
    }

    const auto first = *m_measurementList.constFirst();

    addMetaData("first_systolic",   first.getAttribute("SYS"));
    addMetaData("first_diastolic",  first.getAttribute("DIA"));
    addMetaData("first_pulse",      first.getAttribute("PP"));
    addMetaData("first_start_time", first.getAttribute("AwakeTime"));
    addMetaData("first_end_time",   first.getAttribute("AsleepTime"));


    int validMeasures = 0;
    int systolicTotal = 0;
    int diastolicTotal = 0;
    int pulseTotal = 0;

    for (int i = 1; i < n; i++) { // skip first measurement for avg, total_avg includes the first measure
        auto measure = get(i);
        if (!measure.isValid())
            continue;

        validMeasures++;

        const int systolic = measure.getAttribute("SYS").value().toInt();
        const int diastolic = measure.getAttribute("DIA").value().toInt();
        const int pulse = measure.getAttribute("PP").value().toInt();

        systolicTotal += systolic;
        diastolicTotal += diastolic;
        pulseTotal += pulse;
    }

    if (validMeasures >= 1) {
        const double avgSbpCalc = qRound(systolicTotal * 1.0f / (validMeasures));
        const double avgDbpCalc = qRound(diastolicTotal * 1.0f / (validMeasures));
        const double avgPulseCalc = qRound(pulseTotal * 1.0f / (validMeasures));

        addMetaData("avg_count",     validMeasures);
        addMetaData("avg_systolic",  avgSbpCalc, "mmHg");
        addMetaData("avg_diastolic", avgDbpCalc, "mmHg");
        addMetaData("avg_pulse",     avgPulseCalc, "bpm");
    }

    if (first.isValid()) {
        validMeasures++;

        systolicTotal += getMetaData("first_systolic").toInt();
        diastolicTotal += getMetaData("first_diastolic").toInt();
        pulseTotal += getMetaData("first_pulse").toInt();

        addMetaData("total_avg_systolic",  qRound(systolicTotal * 1.0f / validMeasures), "mmHg");
        addMetaData("total_avg_diastolic", qRound(diastolicTotal * 1.0f / validMeasures), "mmHg");
        addMetaData("total_avg_pulse",     qRound(pulseTotal * 1.0f / validMeasures), "bpm");
        addMetaData("total_avg_count",     validMeasures + 1);
    }
    else
    {
        addMetaData("total_avg_systolic",  qRound(systolicTotal * 1.0f / validMeasures), "mmHg");
        addMetaData("total_avg_diastolic", qRound(diastolicTotal * 1.0f / validMeasures),"mmHg");
        addMetaData("total_avg_pulse",     qRound(pulseTotal * 1.0f / validMeasures),"bpm");
        addMetaData("total_avg_count",     validMeasures);
    }
}

QString WatchBPTest::toString() const
{
    return "";
}
