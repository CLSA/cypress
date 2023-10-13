#ifndef AP_SPINE_MEASUREMENT_H
#define AP_SPINE_MEASUREMENT_H

#include "dxa_measurement.h"




class ApSpineMeasurement : public DXAMeasurement
{
public:
    ApSpineMeasurement();

    QString toString() const override;
    QStringList toStringList(const bool& no_keys = false) const override;

    void simulate() override;

    bool isValidDicomFile(DcmFileFormat& loadedFileFormat) const override;
    bool isValid() const override;

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
