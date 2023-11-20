#ifndef FOREARM_MEASUREMENT_H
#define FOREARM_MEASUREMENT_H

#include "dxa_measurement.h"

#include "../../../dicom/dcm_recv.h"

class ForearmMeasurement : public DXAMeasurement
{
public:
    ForearmMeasurement();

    QString toString() const override;
    QStringList toStringList(const bool& no_keys = false) const override;

    bool isValid() const override;

public:
    bool isValidDicomFile(DicomFile file) const override;
    void addDicomFile(DicomFile);

    DicomFile m_forearmDicomFile{};

    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;

    void simulate() override;

private:
    bool hasForearmFile{false};

    // DXAMeasurement interface
public:
    bool hasAllNeededFiles() const override;
};

#endif // FOREARM_MEASUREMENT_H
