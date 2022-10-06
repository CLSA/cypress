#include "GripStrengthTest.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
//#include "../auxiliary/tracker5util.h"

const q_stringMap GripStrengthTest::testMetaMap = {
    {"Sequence", "sequence"},
    {"Comparison", "comparison"},
    {"MMDescription", "mm_description"},
    {"MaxReps", "max_reps"},
    {"NormType", "norm_type"},
    {"Notes", "notes"},
    {"PrimaryStat", "primary_stat"},
    {"Rate", "rate"},
    {"RestTime", "rest_time"},
    {"Rung", "rung"},
    {"Side", "side"},
    {"Test", "test"},
    {"TestID", "test_id"},
    {"Threshold", "threshold"},
    {"Units", "units"},
    {"Position", "position"},
    {"Rep", "rep"},
    {"Max", "max"},
    {"Avg", "avg"},
    {"CV", "cv"},
};

// the minimum output data keys required from a successful Grip Strength test
//
GripStrengthTest::GripStrengthTest()
{
    m_outputKeyList << "patient_id";
    m_outputKeyList.append(testMetaMap.values());
}

void GripStrengthTest::readGripTestOptions(const QString& gripTestPath)
{
    // Read in test information
    ParadoxReader gripTestReader(gripTestPath);
    QList<QJsonObject> records = gripTestReader.Read();
    foreach(const auto record, records) {
        foreach(const auto tag, testMetaMap.toStdMap()){
            if (record.contains(tag.first)) {
                addMetaData(tag.second, record[tag.first].toVariant());
            }
        }
    }
}

void GripStrengthTest::readGripTestResults(const QString& gripTestDataPath)
{
    // Read test metadata
    ParadoxReader gripTestDataReader(gripTestDataPath);

    // Read test results
    q_paradoxRecords testDataRecords = gripTestDataReader.Read();

    foreach(QJsonObject record, testDataRecords) {
        GripStrengthMeasurement measurement;

        //QString unit = m_metaData.getAttribute(QString("units")).value().toString();
        //if (record.contains("Rep1")) {
        //    qDebug() << Tracker5Util::asKg(record["Rep1"].toInt());
        //    record["Rep1"] = Tracker5Util::asKg(record["Rep1"].toInt());
        //}
        //if (record.contains("Rep2")) {
        //    qDebug() << Tracker5Util::asKg(record["Rep2"].toInt());
        //    record["Rep2"] = Tracker5Util::asKg(record["Rep2"].toInt());
        //}
        //if (record.contains("Rep3")) {
        //    qDebug() << Tracker5Util::asKg(record["Rep3"].toInt());
        //    record["Rep3"] = Tracker5Util::asKg(record["Rep3"].toInt());
        //}

        qDebug() << record;
        measurement.fromRecord(&record);
        if (measurement.isValid()) {
            addMeasurement(measurement);
        }
    }
}

// String representation for debug and GUI display purposes
//
QString GripStrengthTest::toString() const
{
    QString str;
    if(isValid())
    {
      QStringList list;
      foreach(auto measurement, m_measurementList)
      {
        list << measurement.toString();
      }
      str = list.join("\n");
    }
    return str;
}

bool GripStrengthTest::isValid() const
{
    bool okMeta = true;
    foreach(auto key, m_outputKeyList)
    {
      if(!hasMetaData(key))
      {
         okMeta = false;
         break;
       }
    }
    bool okTest = getMeasurementCount() == getExpectedMeasurementCount();
    if(okTest)
    {
      foreach(auto m, m_measurementList)
      {
        if(!m.isValid())
        {
          okTest = false;
          break;
        }
      }
    }
    return okMeta && okTest;
}

QJsonObject GripStrengthTest::toJsonObject() const
{
    QJsonArray jsonArr;
    foreach(auto m, m_measurementList)
    {
      jsonArr.append(m.toJsonObject());
    }
    QJsonObject json;
    if(!metaDataIsEmpty())
    {
      json.insert("test_meta_data",m_metaData.toJsonObject());
    }
    json.insert("test_results",jsonArr);
    return json;
}
