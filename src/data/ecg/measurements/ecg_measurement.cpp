#include "ecg_measurement.h"

#include <QDebug>
#include <QDomElement>
#include <QRandomGenerator>

bool ECGMeasurement::isValid() const
{
    QStringList keys = {
        "diagnosis",
        "ventricular_rate",
        "pq_interval",
        "p_duration",
        "qrs_duration",
        "qt_interval",
        "qtc_interval",
        "rr_interval",
        "pp_interval",
        "p_axis",
        "r_axis",
        "t_axis",
        "qrs_number",
        "p_onset",
        "p_offset",
        "q_onset",
        "q_offset",
        "t_offset"
    };
    bool ok = true;
    foreach(const auto key, keys)
    {
      if(!hasAttribute(key))
      {
        qDebug() << "missing measurement attribute" << key;
        ok = false;
        break;
      }
    }
    return ok;
}

void ECGMeasurement::fromDomNode(const QDomNode &node)
{
  readRestingECGMeasurements(node);
  readInterpretation(node);
}

void ECGMeasurement::readInterpretation(const QDomNode& node)
{
    QDomElement child = node.firstChildElement("Diagnosis");
    if(child.isNull())
      return;

    QDomNodeList list = child.elementsByTagName("DiagnosisText");
    if(list.isEmpty())
      return;

    QStringList arr;
    for(int i=0; i<list.size(); i++)
    {
      QDomElement elem = list.item(i).toElement();
      if(!elem.text().isEmpty())
        arr.append(elem.text());
    }
    if(arr.isEmpty())
      return;

    if(1 < arr.size())
      setAttribute("diagnosis",arr);
    else
      setAttribute("diagnosis",arr.first());
}

void ECGMeasurement::readRestingECGMeasurements(const QDomNode& node)
{
    QDomNodeList list = node.childNodes();
    if(list.isEmpty())
    {
      return;
    }
    QMap<QString,QString> map = {
        {"VentricularRate", "ventricular_rate"},
        {"PQInterval", "pq_interval"},
        {"PDuration", "p_duration"},
        {"QRSDuration", "qrs_duration"},
        {"QTInterval", "qt_interval"},
        {"QTCInterval", "qtc_interval"},
        {"RRInterval", "rr_interval"},
        {"PPInterval", "pp_interval"},
        {"SokolovLVHIndex", "sokolov_lvh_index"}, // not used
        {"PAxis", "p_axis"},
        {"RAxis", "r_axis"},
        {"TAxis", "t_axis"},
        {"QTDispersion", "qt_dispersion"},  // not used
        {"QTDispersionBazett", "qt_dispersion_bazett"}, //not used
        {"QRSNum", "qrs_number"},
        {"POnset", "p_onset"},
        {"POffset", "p_offset"},
        {"QOnset", "q_onset"},
        {"QOffset", "q_offset"},
        {"TOffset", "t_offset"}
    };

    for(int i=0; i<list.count(); i++)
    {
       QDomElement elem = list.item(i).toElement();
       QString tag = elem.tagName();
       if(map.contains(tag))
       {
          QString s = elem.text().toLower();
          if(!s.isEmpty())
          {
            if("no" == s || "yes" == s)
              setAttribute(map[tag],("no" == s ? false : true));
            else
            {
              if(elem.hasAttribute("units"))
                setAttribute(map[tag],s.toInt(),elem.attribute("units"));
              else
                setAttribute(map[tag],s);
            }
          }
       }
    }
}

QString ECGMeasurement::toString() const
{
    QString str;
    if(isValid())
    {
        //TODO: make a string
    }
    return str;
}

QDebug operator<<(QDebug dbg, const ECGMeasurement& item)
{
    const QString str = item.toString();
    if(str.isEmpty())
      dbg.nospace() << "ECG Measurement()";
    else
      dbg.nospace() << "ECG Measurement(" << str << " ...)";
    return dbg.maybeSpace();
}
