#include "weight_measurement.h"

#include <QDateTime>
#include <QDebug>

void WeightMeasurement::fromArray(const QByteArray &arr)
{
    if(!arr.isEmpty())
    {
      QByteArray bytes(arr.simplified());
      QList<QByteArray> parts = bytes.split(' ');
      if(3 <= parts.size())
      {
        setAttribute("WEIGHT", parts[0].toDouble(), QString(parts[1]), 1);
        setAttribute("MODE", QString(parts[2]));
        setAttribute("TIMESTAMP", QDateTime::currentDateTime());
      }
    }
}

bool WeightMeasurement::isValid() const
{
    bool ok = false;
    if(hasAttribute("WEIGHT"))
    {
      float fval = getAttributeValue("WEIGHT").toDouble(&ok);
      ok = ok
        && getAttribute("WEIGHT").hasUnits()
        && hasAttribute("MODE")
        && hasAttribute("TIMESTAMP")
        && 0.0f <= fval;
    }
    return ok;
}

bool WeightMeasurement::isZero() const
{
    return isValid() && 0.0f == getAttributeValue("WEIGHT").toDouble();
}

QString WeightMeasurement::toString() const
{
  QString str;
  if(isValid())
  {
    QStringList list;
    list << getAttribute("WEIGHT").toString();
    list << getAttribute("MODE").toString();
    QDateTime dt = getAttributeValue("TIMESTAMP").toDateTime();
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
