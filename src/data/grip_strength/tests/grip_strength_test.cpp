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
    {"MaxReps", "max_reps"},
    {"NormType", "norm_type"},
    {"PrimaryStat", "primary_stat"},
    {"Rate", "rate"},
    {"RestTime", "rest"},
    {"Rung", "rung"},
    {"Test", "test"},
    {"TestID", "test_id"},
    {"ExamID", "exam_id"},
    {"Threshold", "threshold"},
    {"Units", "units"},
    {"Max", "maximum"},
    {"Avg", "average"},
    {"CV", "cv"},
};

// the minimum output data keys required from a successful Grip Strength test
//
GripStrengthTest::GripStrengthTest()
{
    m_outputKeyList.append(testMetaMap.values());
    setExpectedMeasurementCount(1);
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
        qDebug() << "Does not have metadata key " << key;
         okMeta = false;
         break;
       }
    }
    bool okTest = getMeasurementCount() == getExpectedMeasurementCount();
    if (okTest) {
       foreach (auto m, m_measurementList) {
         if (!m->isValid()) {
               qDebug() << "measurement isn't valid";
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
