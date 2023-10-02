#include "frax_measurement.h"

#include <QDebug>

bool FraxMeasurement::isValid() const
{
    bool ok =
      hasAttribute("TYPE") &&
      hasAttribute("PROBABILITY");
    return ok;
}

QString FraxMeasurement::toString() const
{
    QString s;
    if(isValid())
    {
      QString type = getAttribute("TYPE").toString().replace("_"," ");
      QString p = QString::number(getAttributeValue("PROBABILITY").toDouble(),'f',1);
      s = QString("%1 risk: %2 (%3)").arg(type,p,getAttribute("PROBABILITY").units());
    }
    return s;
}

QDebug operator<<(QDebug dbg, const FraxMeasurement &item)
{
    const QString s = item.toString();
    if (s.isEmpty())
        dbg.nospace() << "Frax Measurement()";
    else
        dbg.nospace() << "Frax Measurement(" << s << " ...)";
    return dbg.maybeSpace();
}
