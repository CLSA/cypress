#ifndef IVA_IMAGING_MEASUREMENT_H
#define IVA_IMAGING_MEASUREMENT_H

#include "dxa_measurement.h"
#include "dicom/dcm_recv.h"

class IVAImagingMeasurement : public DXAMeasurement
{
public:
    IVAImagingMeasurement();

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

    static bool isDicomMeasureFile(DcmFileFormat &file);
    static bool isDicomPRFile(DcmFileFormat &file);
    static bool isDicomOTFile(DcmFileFormat &file);

    bool m_hasDicomFile {false};

    bool hasAllNeededFiles() const override;
    void getScanData(const QSqlDatabase &db, const QString &patientKey, const QString &scanId) override;
};

#endif // IVA_IMAGING_MEASUREMENT_H
