#ifndef IVA_IMAGING_MEASUREMENT_H
#define IVA_IMAGING_MEASUREMENT_H

#include "dxa_measurement.h"
#include "dicom/dcm_recv.h"

class IVAImagingMeasurement : public DXAMeasurement
{
public:
    IVAImagingMeasurement();

    QString toString() const override;

    QStringList toStringList(const bool& no_keys = false) const override;

    bool isValid() const override;

    // DXAMeasurement interface
public:
    void simulate() override;
    bool isValidDicomFile(DicomFile file) const override;

    void addDicomFile(DicomFile);

    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;
};

#endif // IVA_IMAGING_MEASUREMENT_H
