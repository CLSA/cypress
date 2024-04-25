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
    bool isValidDicomFile(DicomFile file) const override;

    void addDicomFile(DicomFile);

    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;

    bool isDicomMeasureFile(DcmFileFormat &file) const;
    bool isDicomPRFile(DcmFileFormat &file) const;
    bool isDicomOTFile(DcmFileFormat &file) const;

    DicomFile m_dicomMeasureFile{};
    DicomFile m_dicomPrFile{};
    DicomFile m_dicomOtFile{};

    bool hasMeasureFile{false};
    bool hasOtFile{false};
    bool hasPrFile{false};

    bool hasAllNeededFiles() const override;
    void getScanData(const QSqlDatabase &db, const QString &patientKey, const QString &scanId) override;
};

#endif // IVA_IMAGING_MEASUREMENT_H
