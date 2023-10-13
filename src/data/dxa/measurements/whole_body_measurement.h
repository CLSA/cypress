#ifndef WHOLE_BODY_MEASUREMENT_H
#define WHOLE_BODY_MEASUREMENT_H

#include "dxa_measurement.h"

class WholeBodyScanMeasurement : public DXAMeasurement
{
public:
    WholeBodyScanMeasurement();

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

#endif // WHOLE_BODY_MEASUREMENT_H
