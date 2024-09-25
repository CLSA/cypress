#include "watch_bp_measurement.h"

WatchBPMeasurement::WatchBPMeasurement() {
    m_outputKeyList << "ID";         // int
    m_outputKeyList << "Patient";    // int
    m_outputKeyList << "SYS";        // int
    m_outputKeyList << "DIA";        // int
    m_outputKeyList << "MAP";        // int
    m_outputKeyList << "PP";         // int
    m_outputKeyList << "cSYS";       // int
    m_outputKeyList << "cDIA";       // int
    m_outputKeyList << "cPP";        // int
    m_outputKeyList << "HR";         // int
    m_outputKeyList << "AFIB";       // int
    m_outputKeyList << "CODE";       // string
    m_outputKeyList << "NOTE";       // string
    m_outputKeyList << "Condition";  // int
    m_outputKeyList << "UpdateTime"; // int
    m_outputKeyList << "Device";     // int
    m_outputKeyList << "Date";       // int
    m_outputKeyList << "BPM";        // string
    m_outputKeyList << "AwakeTime";  // int
    m_outputKeyList << "AsleepTime"; // int
    m_outputKeyList << "Interval1";  // int
    m_outputKeyList << "Interval2";  // int
    m_outputKeyList << "Interval1C"; // int
    m_outputKeyList << "Interval2C"; // int
    m_outputKeyList << "Option";
}

QString WatchBPMeasurement::toString() const
{
    return "";
}

bool WatchBPMeasurement::isValid() const
{
    return true;
}

void WatchBPMeasurement::fromJson(const QJsonObject& data)
{
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        QString key = it.key();
        QJsonValue val = it.value();


        if (key == "UpdateTime") {
            setAttribute("date", QDateTime::fromSecsSinceEpoch(val.toInt()));
        }

        else if (key == "Spare7") {
            setAttribute("reading_number", val.toInt());
        }

        else if (key == "SYS") {
            setAttribute("systolic", val.toInt(), "mmHg");
        }

        else if (key == "DIA") {
            setAttribute("diastolic", val.toInt(), "mmHg");
        }

        else if (key == "HR") {
            setAttribute("pulse", val.toInt(), "bpm");
        }
        else {
            setAttribute(key, val);
        }
    }
}

QJsonObject WatchBPMeasurement::toJsonObject() const
{
    QJsonObject json;
    foreach(const auto x, m_attributes.toStdMap())
    {
      // convert to space delimited phrases to snake_case
      //
      QString key = QString(x.first);
      Value v = x.second;
      QJsonValue jval = QJsonValue::fromVariant(v.value());
      if(v.hasUnits())
      {
          QJsonObject obj;
          obj.insert("value", jval);
          obj.insert("units", v.units());
          json.insert(key,obj);
      }
      else
        json.insert(key,jval);
    }
    return json;
}
