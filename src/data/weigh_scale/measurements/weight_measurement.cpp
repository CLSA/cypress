#include "weight_measurement.h"

#include <QDateTime>
#include <QDebug>
#include <QRandomGenerator>

WeightMeasurement::WeightMeasurement()
{

}

WeightMeasurement::WeightMeasurement(const double weight, const QString unit)
{
    setAttribute("weight", weight, unit);
    setAttribute("mode", "gross");
    setAttribute("timestamp", QDateTime::currentDateTime());
}

void WeightMeasurement::fromArray(const QByteArray &arr)
{
    if(!arr.isEmpty())
    {
      QByteArray bytes(arr.simplified());

      qDebug() << "raw: " << arr;
      qDebug() << "simplified: " << bytes;
      qDebug() << "hex: " << arr.toHex();

      QList<QByteArray> parts = bytes.split(' ');
      if(3 <= parts.size())
      {
          setAttribute("weight", parts[0].toDouble(), QString(parts[1]), 1);
          setAttribute("mode", QString(parts[2]));
          setAttribute("timestamp", QDateTime::currentDateTime());
      }
    }
}

void WeightMeasurement::simulate()
{
    setAttribute("weight",
                 QRandomGenerator::global()->generateDouble() * (100.0 - 50.0) + 50.0,
                 "kg",
                 1);
    setAttribute("mode", "gross");
    setAttribute("timestamp", QDateTime::currentDateTimeUtc());
}

bool WeightMeasurement::isValid() const
{
    bool ok = false;
    if (hasAttribute("weight")) {
        double val = getAttributeValue("weight").toDouble(&ok);
        ok = ok && getAttribute("weight").hasUnits() && hasAttribute("mode")
             && hasAttribute("timestamp") && 0.0f <= val;
    }
    return ok;
}

bool WeightMeasurement::isZero() const
{
    return isValid() && 0.0f == getAttributeValue("weight").toDouble();
}

QString WeightMeasurement::toString() const
{
  QString str;
  if(isValid())
  {
    QStringList list;
    list << getAttribute("weight").toString();
    list << getAttribute("mode").toString();
    QDateTime dt = getAttributeValue("timestamp").toDateTime();
    list << dt.date().toString("yyyy-MM-dd");
    list << dt.time().toString("hh:mm:ss");
    str = list.join(" ");
  }
  return str;
}

QDebug operator<<(QDebug dbg, const WeightMeasurement &item)
{
    const QString s = item.toString();
    if(s.isEmpty())
        dbg.nospace() << "Weight Measurement()";
    else
        dbg.nospace() << "Weight Measurement(" << s << " ...)";
    return dbg.maybeSpace();
}
