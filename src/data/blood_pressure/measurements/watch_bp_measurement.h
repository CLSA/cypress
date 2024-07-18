#ifndef WATCH_BP_MEASUREMENT_H
#define WATCH_BP_MEASUREMENT_H

#include "data/measurement.h"

#include <QJsonObject>

class WatchBPMeasurement : public Measurement
{
public:
    WatchBPMeasurement();
    ~WatchBPMeasurement() = default;

    QString toString() const;
    bool isValid() const;
    QJsonObject toJsonObject() const;

private:
    QList<QString> m_outputKeyList;
};

#endif // WATCH_BP_MEASUREMENT_H
