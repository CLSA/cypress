#include "grip_strength_test.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>
#include <QException>

#include "../../../auxiliary/tracker5_util.h"
//#include "managers/grip_strength/ParadoxReader.h"


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
    m_outputKeyList << "participant_id";
    m_outputKeyList.append(testMetaMap.values());
    setExpectedMeasurementCount(1);
}

//bool GripStrengthTest::readGripTestOptions()
//{
//    return false;
//    // Read in test information
//    ParadoxReader gripTestReader(gripTestPath);
//    QList<QJsonObject> records = gripTestReader.Read();
//    foreach(const auto record, records) {
//        foreach(const auto tag, testMetaMap.toStdMap()){
//            if (record.contains(tag.first)) {
//                addMetaData(tag.second, record[tag.first].toVariant());
//            }
//        }
//    }
//}

bool GripStrengthTest::readMeasurements()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    QList<QJsonObject> testRecords;
    QSqlQuery query;

    db.setHostName(settings.value("instruments/grip_strength/databaseHost", "").toString());
    db.setUserName(settings.value("instruments/grip_strength/databaseUser", "").toString());
    db.setPassword(settings.value("instruments/grip_strength/databasePassword", "").toString());
    db.setDatabaseName(settings.value("instruments/grip_strength/databaseName", "").toString());

    bool ok = db.open();
    if (!ok)
    {
        qDebug() << db.lastError();
        return false;
    }

    try {
        ok = query.exec("SELECT ExamId, TestId, Position, Side, Rep1, Rep2, Rep3, Average, Maximum, CV from [ZGripTestData]");
        if (!ok)
        {
            qDebug() << query.lastError();
            return false;
        }

        while (query.next())
        {
            QJsonObject record;

            record.insert("ExamId",   QJsonValue::fromVariant(query.value(0)));
            record.insert("TestId",   QJsonValue::fromVariant(query.value(1)));
            record.insert("Position", QJsonValue::fromVariant(query.value(2)));
            record.insert("Side", 	  QJsonValue::fromVariant(query.value(3)));

            record.insert("Rep1",  QJsonValue::fromVariant(query.value(4)));
            record.insert("Rep2",  QJsonValue::fromVariant(query.value(5)));
            record.insert("Rep3",  QJsonValue::fromVariant(query.value(6)));

            record.insert("Average", QJsonValue::fromVariant(query.value(7)));
            record.insert("Maximum", QJsonValue::fromVariant(query.value(8)));
            record.insert("CV",      QJsonValue::fromVariant(query.value(9)));

            qDebug() << record;
            testRecords.append(record);
        }

        foreach(QJsonObject record, testRecords) {
            if (record.contains("Rep1")) {
                record["Rep1"] = Tracker5Util::asKg(record["Rep1"].toDouble());
            }
            if (record.contains("Rep2")) {
                record["Rep2"] = Tracker5Util::asKg(record["Rep2"].toDouble());
            }
            if (record.contains("Rep3")) {
                record["Rep3"] = Tracker5Util::asKg(record["Rep3"].toDouble());
            }

            GripStrengthMeasurement measurement;
            measurement.fromRecord(&record);
            if (measurement.isValid()) {
                addMeasurement(measurement);
            }
        }

        db.close();
    }
    catch (QException& e)
    {
        qDebug() << e.what();
        db.close();

        return false;
    }

    return true;
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
