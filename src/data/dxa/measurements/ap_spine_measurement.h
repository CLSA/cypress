#ifndef AP_SPINE_MEASUREMENT_H
#define AP_SPINE_MEASUREMENT_H

#include "dxa_measurement.h"

#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"


class ApSpineMeasurement : public DXAMeasurement
{
public:
    ApSpineMeasurement();

    virtual QString toString() const;

    virtual QStringList toStringList(const bool& no_keys = false) const;

    virtual bool isValid() const;

    virtual QJsonObject toJsonObject() const;

    // DXAMeasurement interface
public:
    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;
};

#endif // AP_SPINE_MEASUREMENT_H
