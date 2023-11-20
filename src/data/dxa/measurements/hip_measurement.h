#ifndef HIP_MEASUREMENT_H
#define HIP_MEASUREMENT_H

#include "dxa_measurement.h"
#include "../../../dicom/dcm_recv.h"

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
    bool isValidDicomFile(DicomFile file) const override;

    DicomFile m_hipDicomFile;

    void addDicomFile(DicomFile);

    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;

private:
    bool m_hasHipFile{false};

    // DXAMeasurement interface
public:
    bool hasAllNeededFiles() const override;
};

#endif // HIP_MEASUREMENT_H
