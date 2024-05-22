#ifndef WHOLE_BODY_MEASUREMENT_H
#define WHOLE_BODY_MEASUREMENT_H

#include "dxa_measurement.h"
#include "dicom/dcm_recv.h"

class WholeBodyScanMeasurement : public DXAMeasurement
{
public:
    WholeBodyScanMeasurement();

    bool isValid() const override;

    QString toString() const override;

    // Static methods
public:
    static bool isValidDicomFile(DicomFile file);
    void getScanData(
        const QSqlDatabase &db,
        const QString &patientKey,
        const QString &scanId
    ) override;
    static bool isWholeBody1(DcmFileFormat &file);
    static bool isWholeBody2(DcmFileFormat &file);

    // DXAMeasurement interface
public:
    void addDicomFile(DicomFile);

    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;

    DicomFile m_dicomFile {};
    bool m_hasDicomFile;

    bool hasAllNeededFiles() const override;
};

#endif // WHOLE_BODY_MEASUREMENT_H
