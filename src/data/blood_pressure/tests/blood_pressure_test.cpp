#include "blood_pressure_test.h"
#include "data/blood_pressure/measurements/blood_pressure_measurement.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

// the minimum output data keys required from a successful a test
//
BloodPressureTest::BloodPressureTest()
{
    m_outputKeyList << "cuff_size";
    m_outputKeyList << "arm_used";

    m_outputKeyList << "first_systolic";
    m_outputKeyList << "first_diastolic";
    m_outputKeyList << "first_pulse";
    m_outputKeyList << "first_start_time";
    m_outputKeyList << "first_end_time";

    m_outputKeyList << "avg_systolic";
    m_outputKeyList << "avg_diastolic";
    m_outputKeyList << "avg_pulse";
    m_outputKeyList << "avg_count";

    m_outputKeyList << "total_avg_systolic";
    m_outputKeyList << "total_avg_diastolic";
    m_outputKeyList << "total_avg_pulse";
    m_outputKeyList << "total_avg_count";

    //setExpectedMeasurementCount(2);
}

// String representation for debug and GUI display purposes
//
QString BloodPressureTest::toString() const
{
    QString str;
    QStringList list;

    foreach (const auto m, m_measurementList) {
        list << m->toString();
    }

    str = list.join("\n");

    return str;
}

bool BloodPressureTest::isValid() const
{
    bool okMeta = true;
    foreach (const auto key, m_outputKeyList) {
        if (!hasMetaData(key)) {
            qDebug() << "missing" << key;
            okMeta = false;
            break;
        }
    }

    bool okTest = 1 <= getMeasurementCount();
    if (okTest) {
        foreach (const auto m, m_measurementList) {
            if (!m->isValid()) {
                okTest = false;
                break;
            }
        }
    }

    qDebug() << "valid: " << (okMeta && okTest);

    return okMeta && okTest;
}

void BloodPressureTest::updateAverage()
{
    if (getMeasurementCount() <= 0) {
        reset();
        return;
    }

    const auto first = *m_measurementList.constFirst();
    addMetaData("first_systolic",   first.getAttribute("systolic"));
    addMetaData("first_diastolic",  first.getAttribute("diastolic"));
    addMetaData("first_pulse",      first.getAttribute("pulse"));
    addMetaData("first_start_time", first.getAttribute("start_time"));
    addMetaData("first_end_time",   first.getAttribute("end_time"));

    const int n = getMeasurementCount();

    int validMeasures = 0;
    int systolicTotal = 0;
    int diastolicTotal = 0;
    int pulseTotal = 0;

    for (int i = 1; i < n; i++) { // skip first measurement for avg, total_avg includes the first measure
        auto measure = get(i);
        if (!measure.isValid())
            continue;

        validMeasures++;

        const int systolic = measure.getAttribute("systolic").value().toInt();
        const int diastolic = measure.getAttribute("diastolic").value().toInt();
        const int pulse = measure.getAttribute("pulse").value().toInt();

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

// String keys are converted to snake_case
//
QJsonObject BloodPressureTest::toJsonObject() const
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

bool BloodPressureTest::verifyDeviceAverage(const int& sbp, const int& dbp, const int& pulse) const
{
    if(hasAverage())
    {
        const int avgSystolic = getMetaData("avg_systolic").toInt();
        const int avgDiastolic = getMetaData("avg_diastolic").toInt();
        const int avgPulse = getMetaData("avg_pulse").toInt();

        if (avgSystolic == sbp && avgDiastolic == dbp && avgPulse == pulse) {
            return true;
        }
        else {
          qDebug() << QString("WARNING: Review data (sbp: %1, dbp: %2, pulse: %3) does not match average data (sbp: %4, dbp: %5, pulse: %6).")
                .arg(sbp).arg(dbp).arg(pulse).arg(avgSystolic).arg(avgDiastolic).arg(avgPulse);
        }
    }

    return false;
}

void BloodPressureTest::reinterpret()
{
    updateAverage();
}

// add the average of the measurements provided by the device and compare
// to computed averages from the preceding stored individual measurements
//
void BloodPressureTest::addDeviceAverage(const int& sbpAvg, const int& dbpAvg, const int& pulseAvg)
{
    int sbpTotal = 0;
    int dbpTotal = 0;
    int pulseTotal = 0;

    if(m_measurementList.isEmpty()) {
        qDebug() << "No measurements to average";
        return;
    }

    // add the first measurement as separate test meta data
    Measurement first = *m_measurementList.first().get();

    addMetaData("first_systolic", first.getAttribute("systolic"));
    addMetaData("first_diastolic", first.getAttribute("diastolic"));
    addMetaData("first_pulse", first.getAttribute("pulse"));
    addMetaData("first_start_time", first.getAttribute("start_time"));
    addMetaData("first_end_time", first.getAttribute("end_time"));

    // skip the first measurement
    int count = 0;
    for(int i = 1; i < m_measurementList.count(); i++)
    {
        auto measurement = qSharedPointerCast<BloodPressureMeasurement>(m_measurementList[i]);
        if (measurement->isValid()) {
            count++;

            sbpTotal += measurement->getSbp();
            dbpTotal += measurement->getDbp();
            pulseTotal += measurement->getPulse();
        }
    }

    if(1 > count)
    {
        qDebug() << "ERROR: not enough measurements to validated device contributed averages";
        return;
    }

    const double avgSbpCalc = qRound(sbpTotal * 1.0f / count);
    const double avgDbpCalc = qRound(dbpTotal * 1.0f / count);
    const double avgPulseCalc = qRound(pulseTotal * 1.0f / count);

    addMetaData("avg_count", QVariant(count));

    bool ok = true;


    if (avgSbpCalc == sbpAvg) {
        addMetaData("avg_systolic", sbpAvg, "mmHg");
    }
    else {
        qDebug() << QString("WARNING: SBP average (%1) does not align with calculated average (%2)").arg(sbpAvg).arg(avgSbpCalc);
        ok = false;
    }

    if (qRound(avgDbpCalc) == dbpAvg) {
        addMetaData("avg_diastolic", dbpAvg, "mmHg");
    }
    else {
        qDebug() << QString("WARNING: DBP average (%1) does not align with calculated average (%2)").arg(dbpAvg).arg(avgDbpCalc);
        ok = false;
    }

    if (qRound(avgPulseCalc) == pulseAvg) {
        addMetaData("avg_pulse", pulseAvg, "bpm");
    }
    else {
        qDebug() << QString("WARNING: Pulse average (%1) does not align with calculated average (%2)").arg(pulseAvg).arg(avgPulseCalc);
        ok = false;
    }

    if (ok) {
        computeTotalAverage(sbpTotal, dbpTotal, pulseTotal);
    }
}

void BloodPressureTest::computeTotalAverage(int sbpTotal, int dbpTotal, int pulseTotal)
{
    if (hasFirstMeasurement()) {
        const int count = m_measurementList.count() + 1;

        sbpTotal += getMetaData("first_systolic").toInt();
        dbpTotal += getMetaData("first_diastolic").toInt();
        pulseTotal += getMetaData("first_pulse").toInt();

        addMetaData("total_avg_systolic", qRound(sbpTotal * 1.0f / count), "mmHg");
        addMetaData("total_avg_diastolic",qRound(dbpTotal * 1.0f / count),"mmHg");
        addMetaData("total_avg_pulse",qRound(pulseTotal * 1.0f / count),"bpm");
        addMetaData("total_avg_count",count);
    }
    else {
        qDebug() << "WARNING: No data found for first measurement when trying to calculate all average data";
    }
}

bool BloodPressureTest::hasFirstMeasurement() const
{
    return
        hasMetaData("first_start_time") &&
        hasMetaData("first_end_time") &&
        hasMetaData("first_systolic") &&
        hasMetaData("first_diastolic") &&
        hasMetaData("first_pulse");
}

bool BloodPressureTest::hasAverage() const
{
    return
        hasMetaData("avg_systolic") &&
        hasMetaData("avg_diastolic") &&
        hasMetaData("avg_pulse") &&
        hasMetaData("avg_count");
}

bool BloodPressureTest::hasTotalAverage() const
{
    return
        hasMetaData("total_avg_systolic") &&
        hasMetaData("total_avg_diastolic") &&
        hasMetaData("total_avg_pulse")&&
        hasMetaData("total_avg_count");
}

void BloodPressureTest::setCuffSize(const QString &size) {
    qDebug() << "blood pressure test: cuff size = " << size;
    addMetaData("cuff_size", QVariant(size));
}

void BloodPressureTest::setSide(const QString &side) {
    qDebug() << "blood pressure test: side = " << side;
    addMetaData("arm_used", QVariant(side));
}

bool BloodPressureTest::armInformationSet() const {
    return hasMetaData("cuff_size")
        && hasMetaData("arm_used");
}
