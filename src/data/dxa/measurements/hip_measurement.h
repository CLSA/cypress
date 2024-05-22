#ifndef HIP_MEASUREMENT_H
#define HIP_MEASUREMENT_H

#include "dxa_measurement.h"
#include "dicom/dcm_recv.h"

class HipMeasurement : public DXAMeasurement
{
public:
    HipMeasurement();

    QString toString() const override;

    bool isValid() const override;

    // DXAMeasurement interface
public:
    static bool isValidDicomFile(DicomFile file);
    void addDicomFile(DicomFile);

    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;

    void getScanData(const QSqlDatabase& db, const QString& patientKey, const QString& scanId) override;

public:
    bool hasAllNeededFiles() const override;

private:
    Side m_side;

    DicomFile m_dicomFile;

    bool m_hasDicomFile;
};

#endif // HIP_MEASUREMENT_H
