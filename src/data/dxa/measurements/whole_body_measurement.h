#ifndef WHOLE_BODY_MEASUREMENT_H
#define WHOLE_BODY_MEASUREMENT_H

#include "dxa_measurement.h"

class WholeBodyScanMeasurement : public DXAMeasurement
{
public:
    WholeBodyScanMeasurement();

    // String representation for debug and GUI display purposes
    //
    virtual QString toString() const;

    virtual QStringList toStringList(const bool& no_keys = false) const;

    virtual bool isValid() const;

    // String keys are converted to snake_case
    //
    virtual QJsonObject toJsonObject() const;

    // DXAMeasurement interface
public:
    bool isValidDicomFile(DcmFileFormat &dicomFileFormat) const;
    Side getSide();
    quint8 getScanType();
    QString getName();
    QString getBodyPartName();
    QString getRefType();
    QString getRefSource();
};

#endif // WHOLE_BODY_MEASUREMENT_H
