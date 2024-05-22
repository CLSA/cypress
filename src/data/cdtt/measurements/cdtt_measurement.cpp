#include "cdtt_measurement.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QRandomGenerator>

bool CDTTMeasurement::isValid() const
{
    qDebug() << "is valid";

    bool ok =
      hasAttribute("trial") &&
      hasAttribute("stimulus") &&
      hasAttribute("response");
    return ok;
}

QString CDTTMeasurement::toString() const {
    QString str;
    if (isValid()) {
        str = QString("trial #%1: stimulus [%2], response [%3]").arg(
          getAttribute("trial").toString(),
          getAttribute("stimulus").toString(),
          getAttribute("response").toString());
    }
    return str;
}

QDebug operator<<(QDebug dbg, const CDTTMeasurement &item) {
    const QString measurementStr = item.toString();
    if(measurementStr.isEmpty())
        dbg.nospace() << "CDTT Measurement()";
    else
        dbg.nospace() << "CDTT Measurement(" << measurementStr << " ...)";

    return dbg.maybeSpace();
}
