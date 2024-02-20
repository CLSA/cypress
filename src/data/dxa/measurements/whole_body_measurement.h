#ifndef WHOLE_BODY_MEASUREMENT_H
#define WHOLE_BODY_MEASUREMENT_H

#include "dxa_measurement.h"
#include "../../../dicom/dcm_recv.h"

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
    bool isValidDicomFile(DicomFile file) const override;

    void addDicomFile(DicomFile);

    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;

    DicomFile m_wholeBody1{};
    DicomFile m_wholeBody2{};

    void getScanData(const QSqlDatabase &db, const QString &patientKey, const QString &scanId) override;

private:
    bool isWholeBody1(DcmFileFormat &file) const;
    bool isWholeBody2(DcmFileFormat &file) const;

    bool hasWholeBody1File{false};
    bool hasWholeBody2File{false};

    // DXAMeasurement interface
public:
    bool hasAllNeededFiles() const override;
};

#endif // WHOLE_BODY_MEASUREMENT_H
