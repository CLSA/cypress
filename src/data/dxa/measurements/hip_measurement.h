#ifndef HIP_MEASUREMENT_H
#define HIP_MEASUREMENT_H

#include "dxa_measurement.h"

class HipMeasurement : public DXAMeasurement
{
public:
    HipMeasurement();

    QString toString() const override;

    QStringList toStringList(const bool& no_keys = false) const override;

    bool isValid() const override;

    // DXAMeasurement interface
public:
    void simulate() override;
    bool isValidDicomFile(DcmFileFormat &dicomFileFormat) const override;
    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;
};

#endif // HIP_MEASUREMENT_H
