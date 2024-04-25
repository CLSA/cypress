#ifndef AP_SPINE_MEASUREMENT_H
#define AP_SPINE_MEASUREMENT_H

#include "dicom/dcm_recv.h"
#include "dxa_measurement.h"


class ApSpineMeasurement : public DXAMeasurement
{
public:
    ApSpineMeasurement();

    QString toString() const override;
    QStringList toStringList(const bool& no_keys = false) const override;

    void addDicomFile(DicomFile);

    bool isValidDicomFile(DicomFile file) const override;
    bool isValid() const override;

    // DXAMeasurement interface
public:
    DicomFile m_apSpineFile;

    void getScanData(const QSqlDatabase& db, const QString& patientKey, const QString& scanId) override;

    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;

    // DXAMeasurement interface
public:
    bool m_hasApSpineFile { false };

    bool hasAllNeededFiles() const override;
};

#endif // AP_SPINE_MEASUREMENT_H
