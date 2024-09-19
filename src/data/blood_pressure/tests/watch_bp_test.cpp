#include "watch_bp_test.h"

#include <QJsonArray>

#include "data/blood_pressure/measurements/watch_bp_measurement.h"

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

void WatchBPTest::fromJson(const QJsonObject& data)
{
    const QJsonArray& measures = data.value("measurements").toArray();

    for (auto it = measures.begin(); it != measures.end(); ++it)
    {
        const QJsonObject& measureData = (*it).toObject();

        QSharedPointer<WatchBPMeasurement> measure(new WatchBPMeasurement);

        measure->fromJson(measureData);

        addMeasurement(measure);
    }

    updateAverage();
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
    return TestBase::toJsonObject();
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
    addMetaData("first_pulse",      first.getAttribute("HR"));
    addMetaData("first_start_time", first.getAttribute("Date"));
    addMetaData("first_end_time",   first.getAttribute("Date"));

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
        const int pulse = measure.getAttribute("HR").value().toInt();

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
