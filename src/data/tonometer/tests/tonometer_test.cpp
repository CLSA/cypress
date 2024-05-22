#include "tonometer_test.h"
#include "auxiliary/Utilities.h"

#include <QDateTime>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>

const q_stringMap TonometerTest::metaLUT = TonometerTest::initMetaLUT();

q_stringMap TonometerTest::initMetaLUT()
{
  q_stringMap map;
  map["id"] = "ID";                             // in "ID" from Patients table
  map["date_of_birth"] = "BirthDate";           // in "BirthDate" from Patients table
  map["sex"] = "Sex";                           // in "Sex" from Patients table
  map["measure_number"] = "MeasureNumber";      // int "MeasureNumber"
  map["session_datetime"] = "SessionDate";      // datetime "SessionDate"
  map["patient_id"] = "PatientID";              // int "PatientID" primary key in Patients table
  map["ora_serial_number"] = "ORASerialNumber"; // string (unique to site) "ORASerialNmber"
  map["ora_software"] = "ORASoftware";          // string "ORASoftware"
  map["pc_software"] = "PCSoftware";            // string "PCSoftware"
  map["meds"] = "Meds";                         // string (not applicable? never used) "Meds"
  map["conditions"] = "Conditions";             // string (not applicable? never used) "Conditions"
  map["notes_1"] = "Notes1";                    // string (not applicable? never used) "Notes1"
  map["notes_2"] = "Notes2";                    // string (not applicable? never used) "Notes2"
  map["notes_3"] = "Notes3";                    // string (not applicable? never used) "Notes3"
  map["m_g2"] = "m_G2";                         // double (not applicable? always 6.711 for both eyes) "m_G2"
  map["b_g2"] = "b_G2";                         // double (not applicable? always 68) "b_G2"
  map["m_g3"] = "m_G3";                         // double (not applicable? always 4.444) "m_G3"
  map["b_g3"] = "b_G3";                         // double (not applicable? always -22.9) "b_G3"
  map["iop_cc_coef"] = "iop_cc_coef";           // double (not applicable? always 0.43) "iop_cc_coef"
  map["crf_coef"] = "crf_coef";                 // double (not applicable? always 0.7) "crf_coef"
  map["m_abc"] = "m_ABC";                       // double (range 1.03 - 1.09) "m_ABC"
  map["b_abc"] = "b_ABC";                       // double (range -23.85 - -3.42) "b_ABC"
  map["b_pp"] = "b_PP";                         // double (not applicable? always 6.12) "b_PP"
  map["best_weighted"] = "BestWeighted";        // uint (not applicable? always 0 or false) "BestWeighted"
  return map;
}

TonometerTest::TonometerTest()
{
    // exam meta data
    m_outputKeyList = TonometerTest::metaLUT.keys();
}

void TonometerTest::fromVariantMapList(const QList<QVariantMap>& variantMapList)
{
    QVariantMap first = variantMapList.first();

    for (auto it = metaLUT.cbegin(); it != metaLUT.cend(); ++it) {
        addMetaData(it.key(), first[it.value()]);
    }


    foreach (auto variantMap, variantMapList) {
        QSharedPointer<TonometerMeasurement> m(new TonometerMeasurement);
        m->fromVariant(variantMap);
        if (m->isValid()) {
           addMeasurement(m);
        }
    }
}

QStringList TonometerTest::getMeasurementStrings(const QString &side) const
{
    QStringList list;
    if (isValid()) {
        foreach (const auto m, m_measurementList) {
          if (side == m->getAttribute("side").toString()) {
            q_stringMap::const_iterator it = TonometerMeasurement::variableLUT.constBegin();
            while(it != TonometerMeasurement::variableLUT.constEnd())
            {
               QString key = it.key();
               it++;
               if("side" == key) continue;
               list << QString("%1: %2").arg(key, m->getAttribute(key).toString());
            }
          }
        }
    }
    return list;
}

// String representation for debug purposes
//
QString TonometerTest::toString() const
{
    QString str;
    if(isValid())
    {
      QStringList list;
      foreach (const auto m, m_measurementList) {
          list << m->toString();
      }
      str = list.join("\n");
    }
    return str;
}

bool TonometerTest::isValid() const
{
    bool okMeta = true;
    foreach(const auto key, m_outputKeyList)
    {
        if(!hasMetaData(key))
        {
            qDebug() << "meta data is not valid" << key;
            okMeta = false;
            break;
        }
    }

    bool okTest = getMeasurementCount() == getExpectedMeasurementCount();
    if (okTest) {
        foreach (const auto m, m_measurementList) {
            if (!m->isValid()) {
                qDebug() << "measurement is not valid";
                okTest = false;
                break;
            }
        }
    }
    return okMeta && okTest;
}

QJsonObject TonometerTest::toJsonObject() const
{
    QJsonObject value{};

    QJsonArray jsonArr{};
    foreach (const auto m, m_measurementList) {
       jsonArr.append(m->toJsonObject());
    }

    QJsonObject deviceJson;
    QJsonObject metaJson = m_metaData.toJsonObject();

    deviceJson.insert("ora_serial_number",metaJson.take("ora_serial_number"));
    deviceJson.insert("ora_software",metaJson.take("ora_software"));
    deviceJson.insert("pc_software",metaJson.take("pc_software"));

    value.insert("device_data", deviceJson);
    value.insert("metadata", metaJson);

    value.insert("results", jsonArr);
    value.insert("manual_entry", getManualEntryMode());

    return value;
}
