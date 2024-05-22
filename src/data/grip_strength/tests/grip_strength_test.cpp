#include "grip_strength_test.h"
#include "auxiliary/tracker5_util.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>
#include <QException>
#include <QRandomGenerator>



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
    setExpectedMeasurementCount(2);
}

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

            record.insert("exam_id",   QJsonValue::fromVariant(query.value(0)));
            record.insert("test_id",   QJsonValue::fromVariant(query.value(1)));
            record.insert("position", QJsonValue::fromVariant(query.value(2)));
            record.insert("side", 	  QJsonValue::fromVariant(query.value(3)));

            record.insert("rep1",  QJsonValue::fromVariant(query.value(4)));
            record.insert("rep2",  QJsonValue::fromVariant(query.value(5)));
            record.insert("rep3",  QJsonValue::fromVariant(query.value(6)));

            record.insert("average", QJsonValue::fromVariant(query.value(7)));
            record.insert("maximum", QJsonValue::fromVariant(query.value(8)));
            record.insert("cv",      QJsonValue::fromVariant(query.value(9)));

            qDebug() << record;
            testRecords.append(record);
        }

        foreach(QJsonObject record, testRecords) {
            if (record.contains("rep1")) {
                record["rep1"] = Tracker5Util::asKg(record["rep1"].toDouble());
            }
            if (record.contains("rep2")) {
                record["rep2"] = Tracker5Util::asKg(record["rep2"].toDouble());
            }
            if (record.contains("rep3")) {
                record["rep3"] = Tracker5Util::asKg(record["rep3"].toDouble());
            }

            QSharedPointer<GripStrengthMeasurement> measurement(new GripStrengthMeasurement);
            measurement->fromRecord(&record);
            if (measurement->isValid()) {
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
      foreach (auto measurement, m_measurementList) {
            list << measurement->toString();
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
    if (okTest) {
       foreach (auto m, m_measurementList) {
         if (!m->isValid()) {
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
    foreach (auto m, m_measurementList) {
       jsonArr.append(m->toJsonObject());
    }

    QJsonObject valuesObject {};

    valuesObject.insert("metadata", m_metaData.toJsonObject());
    valuesObject.insert("results", jsonArr);
    valuesObject.insert("manual_entry", getManualEntryMode());

    return valuesObject;
}
