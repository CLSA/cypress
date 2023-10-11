#ifndef HIP_MEASUREMENT_H
#define HIP_MEASUREMENT_H

#include "dxa_measurement.h"

class HipMeasurement : public DXAMeasurement
{
public:
    HipMeasurement();

    // String representation for debug and GUI display purposes
    //
    virtual QString toString() const override;

    virtual QStringList toStringList(const bool& no_keys = false) const override;

    virtual bool isValid() const override;

    // String keys are converted to snake_case
    //
    virtual QJsonObject toJsonObject() const override;

    // DXAMeasurement interface
public:
    bool isValidDicomFile(DcmFileFormat &dicomFileFormat) const override;
    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;
};

#endif // HIP_MEASUREMENT_H
